/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
//		 							TCL function prototypes 				   																														 
/*************************************************************************************/
#include <tcl.h>

#define NRM  "\x1B[0m"		// Normal Color
#define RED  "\x1B[31m"		// Red Color
#define GRN  "\x1B[32m"		// Green Color
#define YEL  "\x1B[33m"		// Yellow Color
#define BLU  "\x1B[34m"		// Blue Color
#define MAG  "\x1B[35m"		// Magenta Color
#define CYN  "\x1B[36m"		// Cyan Color
#define WHT "\x1B[37m"	// White Color

Tcl_Interp *interpreter;

// function prototypes //

// identify or return the name of the binary file containing the application //
void find_executable(char *);

// Tcl_CreateInterp creates a new interpreter structure and returns a token for it //
int init_interpreter();

// delete created interpreter //
int del_interpreter();

// display the given directory's containts //
int ls(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

// display the contents of a file //
int less(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

// //
int cube_intersect_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int distance_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int supercube_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int cube_cover_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int sharp_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int sharp(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int off_f(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);
