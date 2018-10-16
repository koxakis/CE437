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

	Tcl_CreateObjCommand(interpreter, "less", less, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "ls", ls, NULL, NULL);

	if (Tcl_Init(interpreter) != TCL_OK)
	{
		fprintf(stderr, "!!!Error while creating interpreter  \n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
	// define new tcl comands //
}

int less(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	char command[] = "less";
	char *arguments = NULL;
	char *com_command = NULL;
	int file_length=0;

	// protect from segmentation faults by checking the arguments //
	if (argc != 2)
	{
		fprintf(stderr, "!!! (less) not enough arguments \n less <file> \n");
		return TCL_ERROR;
	}
	// get filename argument //
	// get length of file name from tcl environment global //
	// through Tcl_GetStringFromObj which returns a nonNull length from the given pinter //
	arguments = Tcl_GetStringFromObj(argv[1], &file_length);
	if (arguments == NULL)
	{
		fprintf(stderr, "!!!Error while aquaring arguments  \n");
		return TCL_ERROR;
	}
	// allocate memory for system command //
	com_command = (char *) malloc((file_length ) * sizeof(char));
	if (com_command == NULL)
	{
		fprintf(stderr, "!!!Error in memory allocation \n");
		return TCL_ERROR;
	}

	// compine command and args to send to System //
	sprintf(com_command, "%s %s", command, arguments);
	system(com_command);

	return TCL_OK;
}

int ls(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	char command[] = "ls";
	char *arguments = NULL;
	char *ls_path = NULL;
	char *com_command = NULL;
	int dir_length=0;
	int arg_length=0;

	// protect from segmentation faults by checking the arguments //
	if ( (argc > 3) || (argc < 2) )
	{
		fprintf(stderr, "!!! (ls) not enough arguments \n ls <arg> <directory> \nor ls <directory>\n");
		return TCL_ERROR;
	}

	if ( argc == 3) {
		// get ls argument //
		arguments = Tcl_GetStringFromObj(argv[1], &arg_length);
		if (arguments == NULL)
		{
			fprintf(stderr, "!!!Error while aquaring arguments  \n");
			return TCL_ERROR;
		}
	
		// get ls path //
		ls_path = Tcl_GetStringFromObj(argv[2], &dir_length);
		if (ls_path == NULL)
		{
			fprintf(stderr, "!!!Error while aquaring arguments  \n");
			return TCL_ERROR;
		}

		com_command = (char *) malloc((arg_length + dir_length) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, "!!!Error in memory allocation \n");
			return TCL_ERROR;
		}
		sprintf(com_command, "%s %s %s", command, arguments, ls_path);
	} 
	else if ( argc == 2)
	{
		// get ls path //
		ls_path = Tcl_GetStringFromObj(argv[1], &dir_length);
		if (ls_path == NULL)
		{
			fprintf(stderr, "!!!Error while aquaring arguments  \n");
			return TCL_ERROR;
		}

		// check if second argument is not a directory //
		if ( ls_path[0] == '-' )
		{
			fprintf(stderr, "!!! (ls) missing directory\n ls <directory>\n");
			return TCL_ERROR;
		}

		com_command = (char *) malloc((dir_length) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, "!!!Error in memory allocation \n");
			return TCL_ERROR;
		}	
		sprintf(com_command, "%s %s", command, ls_path);
	}

	// send command to System //
	system(com_command);

	return TCL_OK;
}