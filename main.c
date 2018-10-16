/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 											   																														 
/*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "tcl/tcl_functions.h"
#include "readline/readline_functions.h"


int main(int argc, char *argv[])
{

	int expansion_res;
	int tcl_res;
	char *text_expantion;
	// line for readline use //
	char *line=NULL;
	char *clean_line=NULL;

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
	printf("CAD1 shell		\n");
	printf("Nikolas Koxenoglou	\n");
	printf("*****************************\n");
	
	while(1)
	{
		// make shell green and arrow white //
		line = readline("\x1B[32m CAD1_shell \x1B[37m-> ");

		// if line is null quit //
		if (line == NULL)
		{
			fprintf(stderr, "\x1B[31m!!!Error in line \n");
			return EXIT_FAILURE;
		}

		// clean whitespaces from input //
		clean_line = stripwhite(line);

		// expand history //
		expansion_res = history_expand(line, &text_expantion);
		
		// in case of history expantion error //
		if ( expansion_res == -1)
		{
			fprintf(stderr, "\x1B[31m!!!Error in history expanding: %s\n", text_expantion);
			return EXIT_FAILURE;

		}
		// no expantion needed or do not exeute yet //
		else if ( (expansion_res == 0) || (expansion_res == 2) ) 
		{
			add_history(clean_line);
			// store command //
			strcpy(command, line);
		} 
		else 
		{
			add_history(text_expantion);
			// store command //
			strcpy(command, text_expantion);
		}
		free (text_expantion);
		free (line);
				
		// display exit message for all the available proper exit methods //
		if ( (strcmp(command, "quit") == 0) || (strcmp(command, "exit") == 0) || (strcmp(command, "q") == 0) ) 
		{
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
				fprintf(stderr, "\x1B[31m!!! Error tcl command: %s\n", command);
			} 
			else if (tcl_res == TCL_OK)
			{
				// returns the result for interpreter as a string if there is one //
				if (*Tcl_GetStringResult(interpreter) != '\0')
				{
		        	printf("%s\n", Tcl_GetStringResult(interpreter));
				}
			}
		}	
	}
	free (clean_line);
	//free(commands_list);

	return EXIT_FAILURE;
}
