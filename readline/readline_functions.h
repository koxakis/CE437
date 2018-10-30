/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 						readline and autocomplete function prototypes			   																														 
/*************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#define NRM  "\x1B[0m"		// Normal Color
#define RED  "\x1B[31m"		// Red Color
#define GRN  "\x1B[32m"		// Green Color
#define YEL  "\x1B[33m"		// Yellow Color
#define BLU  "\x1B[34m"		// Blue Color
#define MAG  "\x1B[35m"		// Magenta Color
#define CYN  "\x1B[36m"		// Cyan Color
#define WHT "\x1B[37m"	// White Color


extern char *commands_list[];
// commands list //

// function prototypes //
// complete commands with target for completion text bounded //
// by start and end  //
char **custom_completer(const char *, int, int);

// generate command for tab completetion, use state to find out if //
// we need to start from scratch or just the top of the list //
char *command_gen(const char *, int);

// initialize readline and set custom completer //
void init_readline();

// strip whitespace from the start and end of STRING // 
// return a pointer into STRING. //
char *stripwhite (char *);

