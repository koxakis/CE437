/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 											   																														 
/*************************************************************************************/

#include <tcl8.6/tcl.h>
#include <stdlib.h>

#include "tcl_functions.h"

// identify or return the name of the binary file containing the application //
void find_executable(char *argv)
{
	Tcl_FindExecutable(argv);
}

// Tcl_CreateInterp creates a new interpreter structure and returns a token for it //
int init_interpreter()
{
	interpreter = NULL;
	interpreter = Tcl_CreateInterp();


	if (Tcl_Init(interpreter) != TCL_OK)
	{
		fprintf(stderr, "!!!Error while creating interpreter  \n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
	// define new tcl comands //
	//Tcl_CreateObjCommand(interpreter, "less", less, NULL, NULL);
	//Tcl_CreateObjCommand(interpreter, "ls", ls, NULL, NULL);
}

int less()
{
	return TCL_OK;
}

int ls()
{
	return TCL_OK;
}