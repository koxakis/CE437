/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 								Implemented TCL-8.6 functions 			   																														 
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
		
	// define new tcl comands //
	Tcl_CreateObjCommand(interpreter, "less", less, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "ls", ls, NULL, NULL);

	if (Tcl_Init(interpreter) == TCL_ERROR)
	{
		fprintf(stderr, "\x1B[31m!!!Error while creating interpreter  \n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

// delete created interpreter //
int del_interpreter()
{
	Tcl_DeleteInterp(interpreter);
	return EXIT_SUCCESS;
}

// display the contents of a file //
int less(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	char command[] = "less";
	char *arguments = NULL;
	char *com_command = NULL;
	int file_length=0;

	// protect from segmentation faults by checking the arguments //
	if (argc != 2)
	{
		fprintf(stderr, "\x1B[31m!!! (less) not enough arguments \n\x1B[37m less <file> \n");
		return TCL_ERROR;
	}
	// get filename argument //
	// get length of file name from tcl environment global //
	// through Tcl_GetStringFromObj which returns a nonNull length from the given pinter //
	arguments = Tcl_GetStringFromObj(argv[1], &file_length);
	if (arguments == NULL)
	{
		fprintf(stderr, "\x1B[31m!!!Error while aquaring arguments  \n");
		return TCL_ERROR;
	}
	// allocate memory for system command //
	com_command = (char *) malloc((file_length ) * sizeof(char));
	if (com_command == NULL)
	{
		fprintf(stderr, "\x1B[31m!!!Error in memory allocation \n");
		return TCL_ERROR;
	}

	// combine command and args to send to System //
	sprintf(com_command, "%s %s", command, arguments);
	system(com_command);

	free (com_command);
	return TCL_OK;
}

// display the given directory's containts //
int ls(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	char command[] = "ls";
	char *arguments = NULL;
	char *ls_path = NULL;
	char *com_command = NULL;
	int dir_length=0;
	int arg_length=0;

	// protect from segmentation faults by checking the arguments //
	if ( (argc > 3) )
	{
		fprintf(stderr, "\x1B[31m!!! (ls) not enough arguments \n\x1B[37m ls <arg> <directory> \nor ls <directory>\n");
		return TCL_ERROR;
	}

	if ( argc == 3) {
		// get ls argument //
		arguments = Tcl_GetStringFromObj(argv[1], &arg_length);
		if (arguments == NULL)
		{
			fprintf(stderr, "\x1B[31m!!!Error while aquaring arguments  \n");
			return TCL_ERROR;
		}
		// check if second argument is an ls argument //
		if ( arguments[0] != '-' )
		{
			fprintf(stderr, "\x1B[31m!!! (ls) missing argument\n\x1B[37m ls <arguments> <directory>\n");
			return TCL_ERROR;
		}
	
		// get ls path //
		ls_path = Tcl_GetStringFromObj(argv[2], &dir_length);
		if (ls_path == NULL)
		{
			fprintf(stderr, "\x1B[31m!!!Error while aquaring path  \n");
			return TCL_ERROR;
		}

		com_command = (char *) malloc((arg_length + dir_length) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, "\x1B[31m!!!Error in memory allocation \n");
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
			fprintf(stderr, "\x1B[31m!!!Error while aquaring path  \n");
			return TCL_ERROR;
		}

		com_command = (char *) malloc((dir_length) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, "\x1B[31m!!!Error in memory allocation \n");
			return TCL_ERROR;
		}	
		sprintf(com_command, "%s %s", command, ls_path);
	}
	else if ( argc == 1)
	{
		com_command = (char *) malloc((3) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, "\x1B[31m!!!Error in memory allocation \n");
			return TCL_ERROR;
		}	
		sprintf(com_command, "%s", command);		
	}

	// send command to System //
	system(com_command);

	free(com_command);
	return TCL_OK;
}