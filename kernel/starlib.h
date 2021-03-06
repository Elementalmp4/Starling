// constants
#define RAND_MAX 32767
#define ENTER_DOWN 28
#define LEFT_SHIFT_DOWN 42
#define LEFT_SHIFT_UP 170
#define RIGHT_SHIFT_DOWN 54
#define RIGHT_SHIFT_UP 182
#define BACKSPACE_DOWN 14

#define bool _Bool
#define true 1
#define false 0

// functions
void *mem_cpy(char *src, char *dest, int num_of_bytes);
int strlen(const char *str);
void base_convert(int num, int base);
void print_number(int num, int base);
void print_n(int num);
void print(char *str);
void print_c(char ch);
void slow_print(char *str, int delay);
int rand(void);
void srand(unsigned int seed);
int rand_range(int lower, int upper);
void sleep(unsigned int ms);
void halt(void);
char *read(void);
int hex_to_int(char hex);

//Jack.
void jack(void);