/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 											   																														 
/*************************************************************************************/
#include <tcl8.6/tcl.h>

Tcl_Interp *interpreter;

// function prototypes //

// identify or return the name of the binary file containing the application //
void find_executable(char *);

// Tcl_CreateInterp creates a new interpreter structure and returns a token for it //
void init_interpreter();