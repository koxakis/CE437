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

#include "readline_functions.h"

char *commands_list[]= {
	// my commands //
	"ls", "less", "history", "quit"
	// some extra for convenience //
	,"exit" 
	,NULL
};

// initialize readline, set custom completer and history//
void init_readline(){

	// use the default file name completer //
	rl_completion_entry_function = NULL;

	rl_attempted_completion_function = custom_completer;

	rl_completion_append_character = '\0';

	using_history();
}

// complete commands with target for completion text bounded //
// by start and end  //
char **custom_completer(const char *text, int start, int end)
{
	char **match;

	match = (char **)NULL;

	// detect if the word to be completed is a command ( word at start of line)
	// or it is the name of a file in current directory 
	if (!start) {
		match = rl_completion_matches(text, command_gen);
	}
	// if null then call default readline //
	return match;
}

// generate command for tab completetion, use state to find out if //
// we need to start from scratch or just the top of the list //
char *command_gen(const char *text, int state)
{
	// index is static in order to keep state //
	static int index_list;
	// input text length //
	static int length;

	char *name;

	// initialize word, if this is a new word to complete, and set //
	// set index to 0 //
	if (!state) {
		index_list = 0;
		length = strlen(text);
	}

	// match name from command list until NULL //
	while( (name = commands_list[index_list]) )
	{
		index_list++;

		// should a much be found , return it //
		if ( strncmp(name, text, length) == 0 ) 
		{
			// if found, return duplicated string //
			return strdup(name);
		}
	}
	
	// return NULL when reached the end //
	return NULL;
}

// Strip whitespace from the start and end of STRING.  Return a pointer
//   into STRING. //
char *stripwhite (char *string)
{
   register char *s, *t;

   for (s = string; isspace (*s); s++)
      ;

   if (*s == 0)
      return (s);

   t = s + strlen (s) - 1;
   while (t > s && isspace (*t))
      t--;
   *++t = '\0';

   return s;
}