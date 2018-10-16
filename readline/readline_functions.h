/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 											   																														 
/*************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>


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

