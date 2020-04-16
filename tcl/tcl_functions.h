
/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
//		 							TCL function prototypes 				   																														 
/*************************************************************************************/

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
//#include "../readline/readline_functions.h"
//#include "../file_functions/parser.h"

#define NRM  "\x1B[0m"		// Normal Color
#define RED  "\x1B[31m"		// Red Color
#define GRN  "\x1B[32m"		// Green Color
#define YEL  "\x1B[33m"		// Yellow Color
#define BLU  "\x1B[34m"		// Blue Color
#define MAG  "\x1B[35m"		// Magenta Color
#define CYN  "\x1B[36m"		// Cyan Color
#define WHT  "\x1B[37m"	// White Color

// define parser fsm states //
#define FIRST_NODE 1
#define SEPERATOR 2
#define SECOND_NODE 3
#define VALUE 4
#define ITERATOR 5

#define DEBUG

Tcl_Interp *interpreter;

typedef struct nodes
{
	char *node_name;
	
	unsigned long node_index;

	unsigned long *predecessor;

	unsigned long predecessor_count;

	unsigned long remaining_predecessor;

	int max_value;

	unsigned long max_predecessor;

	unsigned long remaining_successors;

	unsigned long *successor;

	unsigned long successor_count;

	int *value;

}nodesT;

unsigned long node_count;

nodesT *nodes;


int remaining_nodes;

typedef struct cubes
{
	char *cube;
}cubesT;


typedef struct inter_cube_list { 

	char **inter_cube_res;
	struct inter_cube_list *next;
	struct inter_cube_list *prev;
}inter_cube_list_T;

// function prototypes //

// identify or return the name of the binary file containing the application //
void find_executable(char *);

// Tcl_CreateInterp creates a new interpreter structure and returns a token for it //
int init_interpreter();

// delete created interpreter //
int del_interpreter();

// implemented TCL commands //
// display the given directory's containts //
int ls(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

// display the contents of a file //
int less(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int cube_intersect_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int distance_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int supercube_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int cube_cover_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int sharp_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int sharp(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int off_f(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int read_graph(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int vim(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int draw_graph(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int write_graph(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int graph_critical_path(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int alg_division(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);

int r_kernels(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[]);


// helper TCL functions //
int do_sharp_2(char **, char*, char *, int, int);
int count_zeros(char *, int);
int do_cube_cover(char *, char *, int);
void do_supercube(char *, char *, char *, int, int);
void do_cube_intersect(char *, char *, char *, int , int);
int do_cube_check(int, int);
// import stripwhite from readlune_functions.h //
extern char *stripwhite (char *);

