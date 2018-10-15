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

// commands list //
extern char *commands_list[];

// function prototypes //
// complete commands with target for completion text bounded //
// by start and end  //
char **custom_completer(const char *, int, int);


char *command_gen(const char *, int);
void init_readline();