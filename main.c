/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 											   																														 
/*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "tcl/tcl_functions.h"
#include "readline/readline_functions.h"


char *line;

int main(int argc, char *argv[])
{
	

	find_executable(argv[0]);

	init_interpreter();

	init_readline();

	line = readline("koxe_shell: ");

	return 0;
}
