/*************************************************************************************/
//											CE437   																			  
//									  Nikolas Koxenoglou																		
// 								Implemented TCL-8.6 functions 			   																														 
/*************************************************************************************/

#include <tcl.h>
#include <stdlib.h>
#include <string.h>

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

	Tcl_CreateObjCommand(interpreter, "cube_intersect_2", cube_intersect_2, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "distance_2", distance_2, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "supercube_2", supercube_2, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "cube_cover_2", cube_cover_2, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "sharp_2", sharp_2, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "sharp", sharp, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "off_f", off_f, NULL, NULL);


	if (Tcl_Init(interpreter) == TCL_ERROR)
	{
		fprintf(stderr, RED"!!!Error while creating interpreter  \n"NRM);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

// delete created interpreter //
int del_interpreter()
{
	Tcl_FreeResult(interpreter);
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
		fprintf(stderr, RED"!!! (less) not enough arguments \n\x1B[0m less <file> \n"NRM);
		return TCL_ERROR;
	}
	// get filename argument //
	// get length of file name from tcl environment global //
	// through Tcl_GetStringFromObj which returns a nonNull length from the given pinter //
	arguments = Tcl_GetStringFromObj(argv[1], &file_length);
	if (arguments == NULL)
	{
		fprintf(stderr, RED"!!!Error while aquaring arguments  \n"NRM);
		return TCL_ERROR;
	}
	// allocate memory for system command //
	com_command = (char *) malloc((file_length ) * sizeof(char));
	if (com_command == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
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
		fprintf(stderr, RED"!!! (ls) too much arguments \n\x1B[0m ls <arg> <directory> \nor ls <directory>\n");
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
			fprintf(stderr, RED"!!! (ls) missing argument\n\x1B[0m ls <arguments> <directory>\n");
			return TCL_ERROR;
		}
	
		// get ls path //
		ls_path = Tcl_GetStringFromObj(argv[2], &dir_length);
		if (ls_path == NULL)
		{
			fprintf(stderr, RED"!!!Error while aquaring path  \n"NRM);
			return TCL_ERROR;
		}

		com_command = (char *) malloc((arg_length + dir_length) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
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
			fprintf(stderr, RED"!!!Error while aquaring path  \n"NRM);
			return TCL_ERROR;
		}

		com_command = (char *) malloc((dir_length) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}	
		sprintf(com_command, "%s %s", command, ls_path);
	}
	else if ( argc == 1)
	{
		com_command = (char *) malloc((3) * sizeof(char));
		if (com_command == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n");
			return TCL_ERROR;
		}	
		sprintf(com_command, "%s", command);		
	}

	// send command to System //
	system(com_command);

	free(com_command);
	return TCL_OK;
}

// check if even and length requirements are met //
int cube_check_helper_function(int cube_1_l, int cube_2_l)
{
	if ( cube_1_l != cube_2_l )
	{
		fprintf(stderr, RED"!!!(cube_intersect_2) not same length cubes\n\x1B[0m cube_intersect_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}
	else if ( ((cube_1_l % 2)!=0) || ((cube_2_l % 2)!=0) )
	{
		fprintf(stderr, RED"!!!(cube_intersect_2) not even length cubes\n\x1B[0m cube_intersect_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	return TCL_OK;	
}

// check string and appent accordingly for cube_intersect_2 //
char *cube_intersect_helper_function(char *bit_wise_result, char *cube_1, char *cube_2, int cube_1_length, int cube_2_length)
{
	int i;
	int and_result;

	for (i=0; i<cube_1_length; i++)
	{
		and_result = (cube_1[i] - '0') & (cube_2[i] - '0');	
		if (and_result == 1)
		{
			strcat(bit_wise_result,"1");
		}
		else
		{
			strcat(bit_wise_result,"0");
		}
	}

	return bit_wise_result;
}


int cube_intersect_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{

	char *cube_1=NULL;
	int cube_1_length=0;

	char *cube_2=NULL;
	int cube_2_length=0;

	char *bit_wise_result=NULL;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (cube_intersect_2) missing arguments\n\x1B[0m cube_intersect_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);
	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( cube_check_helper_function(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	// check string and appent accordingly for cube_intersect_2 //
	cube_intersect_helper_function(bit_wise_result, cube_1, cube_2, cube_1_length, cube_2_length);

	// Sets the result for the current command as Tcl_FreeProc TCL_VOLATILE states //
	// Tcl_SetResult will make a copy of the string in dynamically allocated storage // 
	// and arrange for the copy to be the result for the current Tcl command //
	Tcl_SetResult(interpreter, bit_wise_result, TCL_VOLATILE);
	free(bit_wise_result);
	return TCL_OK;
}

int distance_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{

	char *cube_1=NULL;
	int cube_1_length=0;

	char *cube_2=NULL;
	int cube_2_length=0;

	char *bit_wise_result=NULL;
	char *return_total_zeros=NULL;

	int i;
	int total_zeros=0;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (distance_2) missing arguments\n\x1B[0m distance_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( cube_check_helper_function(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}

	// check string and appent accordingly for cube_intersect_2 //
	cube_intersect_helper_function(bit_wise_result, cube_1, cube_2, cube_1_length, cube_2_length);

	for (i=0; i<cube_1_length-1; i=i+2)
	{
		if ((bit_wise_result[i] == '0') && (bit_wise_result[i+1] == '0') )
		{
			total_zeros++;
		}
	}
	return_total_zeros = (char *) calloc(cube_1_length,sizeof(char));
	if (return_total_zeros == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	sprintf(return_total_zeros, "%d", total_zeros);

	// Sets the result for the current command as Tcl_FreeProc TCL_VOLATILE states //
	// Tcl_SetResult will make a copy of the string in dynamically allocated storage // 
	// and arrange for the copy to be the result for the current Tcl command //
	Tcl_SetResult(interpreter, return_total_zeros, TCL_VOLATILE);

	free(bit_wise_result);
	free(return_total_zeros);

	return TCL_OK;
}

void supercube_helper_function(char *bit_wise_result, char *cube_1, char *cube_2, int cube_1_length, int cube_2_length)
{
	int i;
	int or_result=0;
	for (i=0; i<cube_1_length; i++)
	{
		or_result = (cube_1[i] - '0') | (cube_2[i] - '0');	
		if (or_result == 1)
		{
			strcat(bit_wise_result,"1");
		}
		else
		{
			strcat(bit_wise_result,"0");
		}
	}
}

int supercube_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{

	char *cube_1=NULL;
	int cube_1_length=0;

	char *cube_2=NULL;
	int cube_2_length=0;

	char *bit_wise_result=NULL;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (supercube_2) missing arguments\n\x1B[0m supercube_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( cube_check_helper_function(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	// check string and appent accordingly for cube_intersect_2 //
	cube_intersect_helper_function(bit_wise_result, cube_1, cube_2, cube_1_length, cube_2_length);

	bit_wise_result = (char *) calloc(cube_1_length,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	// check string and appent accordingly //
	// IN_FUNC //
	cube_intersect_helper_function(bit_wise_result, cube_1, cube_2, cube_1_length, cube_2_length);

	// Sets the result for the current command as Tcl_FreeProc TCL_VOLATILE states //
	// Tcl_SetResult will make a copy of the string in dynamically allocated storage // 
	// and arrange for the copy to be the result for the current Tcl command //
	Tcl_SetResult(interpreter, bit_wise_result, TCL_VOLATILE);
	free(bit_wise_result);
	return TCL_OK;	
}

int cube_cover_helper_function(char *cube_1, char *cube_2, int cube_1_length)
{
	int i;
	int lesser_found = 0;

	for (i=0; i<cube_1_length; i++)
	{
		// check if cube 1 has less than digit than cube 1 //
		if ( ((cube_1[i]-'0') < (cube_2[i]-'0')) == 1 )
		{
			// if a lesser digit is found increment and break //
			lesser_found++;
			return lesser_found;
		}
	}
	return lesser_found;
}

int cube_cover_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{

	char *cube_1=NULL;
	int cube_1_length=0;

	char *cube_2=NULL;
	int cube_2_length=0;

	char *bit_wise_result=NULL;
	char *is_covered_by=NULL;

	int lesser_found = 0;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (cube_cover_2) missing arguments\n\x1B[0m cube_cover_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( cube_check_helper_function(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	// check string and appent accordingly //
	// IN_FUNC //
	lesser_found = cube_cover_helper_function(cube_1, cube_2, cube_1_length);

	is_covered_by = (char *) calloc(cube_1_length + cube_2_length + 26,sizeof(char));
	if (is_covered_by == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	if (lesser_found > 0)
	{
		sprintf(is_covered_by, "Cube %s does NOT cover %s", cube_1, cube_2);

	} 
	else
	{
		sprintf(is_covered_by, "Cube %s does cover %s", cube_1, cube_2);
	}

	// Sets the result for the current command as Tcl_FreeProc TCL_VOLATILE states //
	// Tcl_SetResult will make a copy of the string in dynamically allocated storage // 
	// and arrange for the copy to be the result for the current Tcl command //
	Tcl_SetResult(interpreter, is_covered_by, TCL_VOLATILE);
	free(is_covered_by);
	free(bit_wise_result);
	return TCL_OK;
}

char **sharp_2_helper_function(char* cube_1, char *cube_2, int cube_1_length, int cube_2_length)
{
	char **final_cubes=NULL;
	int not_cube2[2];
	int check_if_zero[2];
	unsigned long i, j, sync_i_j;


	final_cubes = (char **) calloc(cube_1_length,sizeof(char*));
	if (final_cubes == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		exit(1);
	}

	for (i=0; i<cube_1_length/2; i++)
	{
		final_cubes[i] = (char *) calloc(cube_1_length,sizeof(char));
		if (final_cubes[i] == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			exit(1);
		}
	}

	sync_i_j = 0;
	for (i=0; i<(cube_1_length/2); i++)
	{
		for (j=0; j<(cube_1_length); j=j+2)
		{
			not_cube2[0] = !(cube_2[j]-'0');
			not_cube2[1] = !(cube_2[j+1]-'0');
			
			// check if the result is 00 before assigning it //
			check_if_zero[0] = ( (cube_1[j]-'0') & not_cube2[0] ) ;
			check_if_zero[1] = ( (cube_1[j+1]-'0') & not_cube2[1] ) ;

			// i iterates half the length of j, in order to maintain the two by two check in the cubes //
			// a sync variable is added to i to keep track of the diagonal //
			if (j != (i+sync_i_j) )
			{
				// assign not diagonal values //
				final_cubes[i][j] = cube_1[j];
				final_cubes[i][j+1] = cube_1[j+1];
			}
			else
			{
				// if an illegal 00 is detected, the memory is released and the siring is assigned to \0 //
				if( ( check_if_zero[0] == 0) && ( check_if_zero[1] == 0) ) 
				{
					final_cubes[i] = (char*)realloc(final_cubes[i], sizeof(char) );
					if (final_cubes[i] == NULL)
					{
						fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
						exit(1);
					}
					final_cubes[i] = "\0";
					break;
				}
				else
				{
					final_cubes[i][j] = check_if_zero[0] + '0';
					final_cubes[i][j+1] = check_if_zero[1] + '0';
				}
			}
		}
		// used to keep track of the diagonal //
		sync_i_j++;
	}

	return final_cubes;
	//return TCL_OK;
}

int sharp_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	//tcl_getelements, tcl_listobjgetelement

	char *cube_1=NULL;
	int cube_1_length=0;

	char *cube_2=NULL;
	int cube_2_length=0;

	char **final_cubes=NULL;

	unsigned long i;
	//int return_result_length=0;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (sharp_2) missing arguments\n\x1B[0m sharp_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( cube_check_helper_function(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}
	// perform sharp ( # )
	final_cubes =  sharp_2_helper_function(cube_1, cube_2, cube_1_length, cube_2_length);

	for (i=0; i<(cube_1_length/2); i++ )
	{
		if( strcmp(final_cubes[i],"\0")!=0 )
		{
			printf("%s\n", final_cubes[i]);
		}
	}

	for (i=0; i<(cube_1_length/2); i++ )
	{
		if( strcmp(final_cubes[i],"\0")!=0 )
		{
			free(final_cubes[i]);
		}	
	}
	free(final_cubes);

	return TCL_OK;
}

int sharp(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	//char **sharp_2_cubes=NULL;
	inter_cube_list_T *final_cubes=NULL;
	inter_cube_list_T *curr=NULL;
	inter_cube_list_T *curr_temp=NULL;
	char *cube_1=NULL;
	char *list_f_in=NULL;
	char *token=NULL;
	char delim[2]=",";

	int cube_1_length;
	int list_f_in_length;
	int i;

	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);
	list_f_in = Tcl_GetStringFromObj(argv[2], &list_f_in_length);

	// find the first appearance of delimeter in given list //
	token = strtok(list_f_in, delim);

	final_cubes = (inter_cube_list_T*)malloc(sizeof(inter_cube_list_T));
	if (final_cubes == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		exit(1);
	}
	final_cubes->next=final_cubes;
	final_cubes->prev=final_cubes;

	while (token != NULL)
	{
		// remove any whitespace character from cube //
		token = stripwhite(token);
			// check if even and length requirements are met //
		if( cube_check_helper_function(cube_1_length, (int)strlen(token)) == TCL_ERROR)
		{
			return TCL_ERROR;
		}

		curr = (inter_cube_list_T*)malloc(sizeof(inter_cube_list_T));
		if (curr == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			exit(1);
		}	
		curr->next = final_cubes->next;
		curr->prev = final_cubes;

		curr->next->prev = curr;
		curr->prev->next = curr;

		curr->inter_cube_res = sharp_2_helper_function(cube_1, token, cube_1_length, (int)strlen(token));


		// find next cube in list //
		token = strtok(NULL, delim);
	}

	for(curr=final_cubes->next; curr!=final_cubes; curr= curr->next)
	{
		printf("Result set :\n");
		for(i=0; i<cube_1_length/2; i++)
		{
			if( strcmp(curr->inter_cube_res[i],"\0")!=0 )
			{
				printf("%s \n", curr->inter_cube_res[i]);
			}
		}
	}

	curr= final_cubes->next;
	while (curr != final_cubes)
	{
		for(i=0; i<cube_1_length/2; i++)
		{
			if( strcmp(curr->inter_cube_res[i],"\0")!=0 )
			{
				free(curr->inter_cube_res[i]);
			}
		}
		curr_temp = curr;
		curr = curr->next;
		free(curr_temp);
	}
	free(final_cubes);

	return TCL_OK;
}


int off_f(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	return TCL_OK;
}

