# Starling Development

Welcome to Starling's Developer Documentation! You must be here due to a series of bad choices. 

Currently, the default program just says "WEEE" in all the supported foreground and background colours.

## Libraries

Starling comes with one main library: StarLib. StarLib is the standard library for starling. It contains the following functions:

Starlib can be accessed via `../kernel/starlib.h`

```C
//Copy some data of known size from one memory address to another
void *mem_cpy(char *src, char *dest, int num_of_bytes);

//Get the length of a string
int strlen(const char *str);

//Convert a number to a base
void base_convert(int num, int base);

//Print a number of a known base
void print_number(int num, int base);

//Print a number
void print_n(int num);

//Print a string
void print(char *str);

//Print a character
void print_c(char ch);

//Generate a random number
int rand(void);

//Seed the number generator
void srand(unsigned int seed);

//Sleep for a number of milliseconds
void sleep(unsigned int ms);

//Halts the CPU until an interrupt is received
void halt(void);

//Reads user input and returns it
char *read(void);
```

As well as StarLib, Starling also has some other important libraries which interface with the kernel. For example:

`../kernel/kernel.h`

```C
//Define a function to handle keyboard input.
//This function receives an integer key scancode when called as a parameter.
void set_input_function(void proc_input);
```

`../drivers/monitor_mode_txt.h`

```C
//Set the text foreground colour (0-15);
void set_foreground_colour(int colour);

//Set the text background colour (0-15);
void set_background_colour(int colour);

//Print a string at a specific X,Y on the screen
void print_at(char *str , int col , int row);

//Clear the screen
void clear_screen(void);

//Hide the blinking cursor
void hide_cursor(void);
```

## Writing a program

Your program will be written in C and it will be located in the `prog` folder.

As a bare minimum, you will need your main script to include `main.h` and contain the `run_program()` function.

Here's an example program:

```C
#include "main.h"
#include "../kernel/starlib.h"

int run_program(void)
{
	print("Hello World!\n");
}
```
This example program simply prints "Hello World!" and then does nothing. 

You can create other files and subfolders in the `prog` directory and they will be automatically detected and compiled for you.