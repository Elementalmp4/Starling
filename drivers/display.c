#include <display.h>
#include <low_level_io.h>
#include <starlib.h>

char attribute_byte = GRAY_ON_BLACK;

int fg_colour = 7;
int bg_colour = 0;

bool expecting_colour = false;
char colour_expected = '\0';

unsigned char *get_video_memory() {
	return (unsigned char *)VGA_ADDRESS;
}

void print_char(char character, int col, int row)
{
	unsigned char *vid_mem = get_video_memory();
	int offset;
	
	//Get offset if a position is specified, otherwise get current cursor
	if (col >= 0 && row >= 0)	
		offset = get_scr_offset(col, row);
	else
		offset = get_cursor();

	if (expecting_colour) {
		if (colour_expected == 'f')
			set_foreground_colour(hex_to_int(character));
		else 
			set_background_colour(hex_to_int(character));
		expecting_colour = false;
		return;
	}

	if (character == '\f') {
		expecting_colour = true;
		colour_expected = 'f';
		return;
	}

	if (character == '\b') {
		expecting_colour = true;
		colour_expected = 'b';
		return;
	}

	if (character == '\n') {
		offset = get_scr_offset(MAX_COLS - 1, offset / (2 * MAX_COLS));
	} else {
		vid_mem[offset] = character;
		vid_mem[offset + 1] = attribute_byte;
	}
	
	// go to next character cell
	offset += 2;
	offset = handle_scrolling(offset);
	set_cursor(offset);
}

int get_scr_offset(int col, int row)
{
	/* calculates an offset in VGA memory from col and row */
	int offset = (row * MAX_COLS + col) * 2;
	
	return offset;
}

int get_cursor(void)
{
	/* gets the cursor position from the hardware */
	int offset = 0;
	
	port_byte_out(REG_SCR_CTRL, 0x0E);
	offset = port_byte_in(REG_SCR_DATA) << 8;
	port_byte_out(REG_SCR_CTRL, 0x0F);
	offset |= port_byte_in(REG_SCR_DATA);
	
	// cursor offset is reported as the # of characters
	offset *= 2;
	
	return offset;
}

void set_cursor(int offset)
{
	/* sets the cursor at offset */
	offset /= 2; // convert to char offset
	
	port_byte_out(REG_SCR_CTRL, 0x0E);
	port_byte_out(REG_SCR_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCR_CTRL, 0x0F);
	port_byte_out(REG_SCR_DATA, (unsigned char)offset);
}

void print_at(char *str , int col , int row) 
{
	/* prints str at position */
	int i;
	bool coordinates = false;
	
	// update the cursor if need be
	if ( col >= 0 && row >= 0)
	{
		set_cursor(get_scr_offset(col, row));
		coordinates = true;
	}
	
	// loop chars and print
	if (!coordinates)
		for (i = 0; str[i] != '\0'; ++i)
			print_char(str[i], col, row);
	else
		for (i = 0; str[i] != '\0'; ++i, ++col)
			print_char(str[i], col, row);
}

void clear_screen(void)
{
	unsigned char *vid_mem = get_video_memory();
	bool character = true;
	for (int offset = 0; offset <= (MAX_COLS * MAX_ROWS) * 2; offset++) {
		if (character) {
			vid_mem[offset] = '\0';
			character = false;
		} else character = true;
	}	

	// set cursor to top left
	set_cursor(get_scr_offset(0, 0));
}

int handle_scrolling(int cursor_offset)
{
	// return if no need for scrolling
	if (cursor_offset < MAX_ROWS * MAX_COLS * 2)
		return cursor_offset;
		
	/* move all rows up one row */
	int i;
	for (i = 1; i < MAX_ROWS; ++i)
	{
		mem_cpy((char *)(get_scr_offset(0, i) + VGA_ADDRESS),
				(char *)(get_scr_offset(0, i - 1) + VGA_ADDRESS),
				MAX_COLS * 2);
	}
	
	// blank the last line
	char *lst_ln = (char *)(get_scr_offset(0, MAX_ROWS - 1) + VGA_ADDRESS);
	for (i = 0; i < MAX_COLS; i += 2)
		lst_ln[i] = ' ';
		
	cursor_offset -= 2 * MAX_COLS;
	
	return cursor_offset;
}

char get_char_at(int row, int col)
{
	/* reads a character from row, col position */
	unsigned char *vid_mem = get_video_memory();
	int offset = get_scr_offset(row, col);
	
	return vid_mem[offset];
}

void set_attribute_byte(char attrib)
{
	/* sets the atribute byte - colors and stuff */
	attribute_byte = attrib;
}

char get_attribute_byte(void)
{
	/* gets the current attribute byte */
	return attribute_byte;
}

void hide_cursor(void)
{
	port_word_out(REG_SCR_CTRL, HIDE_CURSOR);
}

void show_cursor(void) {
	port_word_out(REG_SCR_CTRL, SHOW_CURSOR);
}

int format_input_colour(int colour) {
	if (colour < 0) return 0;
	else if (colour > 15) return 15;
	else return colour;
}

void set_foreground_colour(int colour) {
	fg_colour = format_input_colour(colour);
	update_attribute_byte();
}

void remove_last_character() {
	int offset = get_cursor();
	unsigned char *vid_mem = get_video_memory();
	if (offset >= 2) offset = offset - 2;
	vid_mem[offset] = '\0';
	vid_mem[offset + 1] = attribute_byte;
	set_cursor(offset);
}

void set_background_colour(int colour) {
	bg_colour = format_input_colour(colour);
	update_attribute_byte();
}

void update_attribute_byte() {
	set_attribute_byte(fg_colour | bg_colour << 4);
}

char create_attribute_byte(int fg_c, int bg_c) {
	return fg_c | bg_c << 4;
}

void set_attribute_byte_at(char attribute_byte, int row, int col) {
	int offset = get_scr_offset(col, row);
	unsigned char *vid_mem = get_video_memory();
	vid_mem[offset + 1] = attribute_byte;
}