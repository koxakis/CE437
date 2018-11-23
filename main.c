/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 										main program		   																														 
/*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "tcl/tcl_functions.h"
#include "readline/readline_functions.h"

#define NRM  "\x1B[0m"		// Normal Color
#define RED  "\x1B[31m"		// Red Color
#define GRN  "\x1B[32m"		// Green Color
#define YEL  "\x1B[33m"		// Yellow Color
#define BLU  "\x1B[34m"		// Blue Color
#define MAG  "\x1B[35m"		// Magenta Color
#define CYN  "\x1B[36m"		// Cyan Color
#define WHT "\x1B[37m"	// White Color

int main(int argc, char *argv[])
{

	int expansion_res;
	int tcl_res;
	char *text_expantion=NULL;
	// line for readline use //
	char *line=NULL;
	char *clean_line=NULL;

	char hostname[HOST_NAME_MAX];
	char username[LOGIN_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);
	getlogin_r(username, LOGIN_NAME_MAX);

	// readline/hostory.h internal struct //
	HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //

	// current command //
	char command[LINE_MAX]; 
	unsigned long i;

	// identify or return the name of the binary file containing the application // 
	find_executable(argv[0]);

	// Tcl_CreateInterp creates a new interpreter structure and returns a token for it //
	init_interpreter();

	// initialize readline and set custom completer //
	init_readline();

	printf("*****************************\n");
	printf("CE437\n");
	printf("CAD1 TCL Shell\n");
	printf("Nikolas Koxenoglou\n");
	printf("\nYou are logged in as User: %s \n", username);
	printf("On machine with hostname: %s\n", hostname);
	printf("*****************************\n\n");
	
	while(1)
	{
		// make shell green and arrow white //
		line = readline(GRN">TCL_shell £ "NRM);

		// if line is null quit //
		if (line == NULL)
		{
			fprintf(stderr, RED"!!!Error in line \n"NRM);
			return EXIT_FAILURE;
		}

		// clean whitespaces from input //
		clean_line = stripwhite(line);

		// skip tcl evaluation when user presses enter without a command //
		// in order to avoid tcl eval error on empty command //
		if (strlen(clean_line) == 0 )
		{
			continue;
		}

		// expand history //
		expansion_res = history_expand(clean_line, &text_expantion);
		
		// in case of history expantion error //
		if ( expansion_res == -1)
		{
			fprintf(stderr, RED"!!!Error in history expanding: %s\n"NRM, text_expantion);
			return EXIT_FAILURE;

		}
		// no expantion needed or do not exeute yet //
		else if ( (expansion_res == 0) || (expansion_res == 2) ) 
		{
			// don't add empty lines in history
			if ( clean_line && *clean_line)
			{
				add_history(clean_line);
				// store command //
				strcpy(command, clean_line);
			}
		} 
		else 
		{
			// don't add empty lines in history
			if ( clean_line && *clean_line)
			{
				add_history(text_expantion);
				// store command //
				strcpy(command, text_expantion);
			}
		}
		free (text_expantion);

		// display exit message for all the available proper exit methods //
		if ( (strcmp(command, "quit") == 0) || (strcmp(command, "exit") == 0) || (strcmp(command, "q") == 0) ) 
		{
			free (clean_line);
			del_interpreter();
			printf ("\nGood bye !!!\n");
			return EXIT_SUCCESS;
		}
		else if (strcmp(command, "history") == 0) 
		{
			// get history list and store in HIST_ENTRY ** history_list (void)//
			the_history_list = history_list();

			if (the_history_list != NULL)
			{
				i = 0;
				// loop until list finds NULL //
				while (*(the_history_list + i) != NULL)
				{
					printf("\t%ld: %s\n", (i+history_base), (*(the_history_list + i))->line);
					i++;
				}
			}
		}
		else // run the tcl //
		{	
			// evaluate tcl commands //
			tcl_res = Tcl_Eval(interpreter, command);
			if (tcl_res == TCL_ERROR)
			{
				fprintf(stderr, RED"!!! Error tcl command: %s\n"NRM, command);
                // returns the result for interpreter as a string if there is one //
				if (*Tcl_GetStringResult(interpreter) != '\0')
				{
		  		 	printf("%s\n", Tcl_GetStringResult(interpreter));
					// free result //
					Tcl_ResetResult(interpreter);
				}
			} 
			else if (tcl_res == TCL_OK)
			{
				// returns the result for interpreter as a string if there is one //
				if (*Tcl_GetStringResult(interpreter) != '\0')
				{
		  			printf("%s\n", Tcl_GetStringResult(interpreter));
					// free result //
					Tcl_ResetResult(interpreter);
				}
			}
		}	
	}
	free (clean_line);
	return EXIT_FAILURE;
}
