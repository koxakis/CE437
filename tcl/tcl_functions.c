#include <tcl.h>

#include "tcl_functions.h"

// identify or return the name of the binary file containing the application //
void find_executable(char *argv)
{
	Tcl_FindExecutable(argv);
}

// Tcl_CreateInterp creates a new interpreter structure and returns a token for it //
void init_interpreter()
{
	Tcl_Interp *interpreter = NULL;

	interpreter = Tcl_CreateInterp();

	// define new tcl comands //
	//Tcl_CreateObjCommand(interpreter, "less", less, NULL, NULL);
	//Tcl_CreateObjCommand(interpreter, "ls", ls, NULL, NULL);
}