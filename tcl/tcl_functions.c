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
int do_cube_check(int cube_1_l, int cube_2_l)
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
void do_cube_intersect(char *bit_wise_result, char *cube_1, char *cube_2, int cube_1_length, int cube_2_length)
{
	//char *bit_wise_result=NULL;
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

	//return bit_wise_result;
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
		fprintf(stderr, RED"!!! (cube_intersect_2) illegal arguments\n\x1B[0m cube_intersect_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);
	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( do_cube_check(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length+1,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}

	// check string and appent accordingly for cube_intersect_2 //
	do_cube_intersect(bit_wise_result, cube_1, cube_2, cube_1_length, cube_2_length);

	// Sets the result for the current command as Tcl_FreeProc TCL_VOLATILE states //
	// Tcl_SetResult will make a copy of the string in dynamically allocated storage // 
	// and arrange for the copy to be the result for the current Tcl command //
	Tcl_SetResult(interpreter, bit_wise_result, TCL_VOLATILE);
	free(bit_wise_result);
	return TCL_OK;
}

int count_zeros(char *cube, int cube_length)
{
	int i;
	int total_zeros=0;

	for (i=0; i<cube_length-1; i=i+2)
	{
		if ((cube[i] == '0') && (cube[i+1] == '0') )
		{
			total_zeros++;
		}
	}

	return total_zeros;
}

int distance_2(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{

	char *cube_1=NULL;
	int cube_1_length=0;

	char *cube_2=NULL;
	int cube_2_length=0;

	char *bit_wise_result=NULL;
	char *return_total_zeros=NULL;

	int total_zeros=0;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (distance_2) illegal arguments\n\x1B[0m distance_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( do_cube_check(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length+1,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		exit(1);
	}

	// check string and appent accordingly for cube_intersect_2 //
	do_cube_intersect(bit_wise_result, cube_1, cube_2, cube_1_length, cube_2_length);

	total_zeros = count_zeros(bit_wise_result, cube_1_length);

	return_total_zeros = (char *) calloc(cube_1_length+1,sizeof(char));
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

void do_supercube(char *bit_wise_result, char *cube_1, char *cube_2, int cube_1_length, int cube_2_length)
{
	//char *bit_wise_result=NULL;
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
	//return bit_wise_result;
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
		fprintf(stderr, RED"!!! (supercube_2) illegal arguments\n\x1B[0m supercube_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( do_cube_check(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length+1,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		exit(1);
	}

	// check string and appent accordingly //
	// IN_FUNC //
	do_supercube(bit_wise_result,cube_1, cube_2, cube_1_length, cube_2_length);

	// Sets the result for the current command as Tcl_FreeProc TCL_VOLATILE states //
	// Tcl_SetResult will make a copy of the string in dynamically allocated storage // 
	// and arrange for the copy to be the result for the current Tcl command //
	Tcl_SetResult(interpreter, bit_wise_result, TCL_VOLATILE);
	free(bit_wise_result);
	return TCL_OK;	
}

int do_cube_cover(char *cube_1, char *cube_2, int cube_1_length)
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
		fprintf(stderr, RED"!!! (cube_cover_2) illegal arguments\n\x1B[0m cube_cover_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( do_cube_check(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	bit_wise_result = (char *) calloc(cube_1_length+1,sizeof(char));
	if (bit_wise_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	// check string and appent accordingly //
	// IN_FUNC //
	lesser_found = do_cube_cover(cube_1, cube_2, cube_1_length);

	is_covered_by = (char *) calloc(cube_1_length + cube_2_length + 22,sizeof(char));
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

int do_sharp_2(char **final_cubes,char* cube_1, char *cube_2, int cube_1_length, int cube_2_length)
{
	int not_cube2[2];
	int check_if_zero[2];
	unsigned long i, j, sync_i_j;

	sync_i_j = 0;
	for (i=0; i<(cube_1_length/2); i++)
	{
		for (j=0; j<(cube_1_length); j=j+2)
		{
			// invert cube 2 input //
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
						return TCL_ERROR;
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
		// remove duplicate entries from # output //
		for(j=0; j < cube_1_length/2; j++)
		{
			// skip if j is the same as i then this is the string it self //
			if (j==i)
			{
				continue;
			}
			else
			{
				// if the strings are the same and the string is not NULL then reallocate the memory and erase the entry //
				if( (strcmp(final_cubes[i], final_cubes[j]) == 0) && (strcmp(final_cubes[j],"\0")!=0) )
				{
					final_cubes[j] = (char*)realloc(final_cubes[i], sizeof(char) );
					if (final_cubes[j] == NULL)
					{
						fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
						return TCL_ERROR;
					}
					final_cubes[j] = "\0";				
				}
			}
		}
		// used to keep track of the diagonal //
		sync_i_j++;
	}

	return TCL_OK;
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

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (sharp_2) illegal arguments\n\x1B[0m sharp_2 <cube 1> <cube 2>\n");
		return TCL_ERROR;
	}

	// get cube 1 data //
	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);

	// get cube 2 data //
	cube_2 = Tcl_GetStringFromObj(argv[2], &cube_2_length);

	// check if even and length requirements are met //
	if( do_cube_check(cube_1_length, cube_2_length) == TCL_ERROR)
	{
		return TCL_ERROR;
	}

	final_cubes = (char**) calloc(cube_1_length/2,sizeof(char*));
	if (final_cubes == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}

	for (i=0; i<cube_1_length/2; i++)
	{
		final_cubes[i] =(char*) calloc(cube_1_length+1,sizeof(char));
		if (final_cubes[i] == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
	}
	// perform sharp ( # )
	do_sharp_2(final_cubes, cube_1, cube_2, cube_1_length, cube_2_length);

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
	inter_cube_list_T *head=NULL;
	inter_cube_list_T *curr=NULL;
	inter_cube_list_T *curr_temp=NULL;
	char **old_result=NULL;
	char **temp_result=NULL;
	char *token=NULL;
	char *cube_1=NULL;
	char *list_f_in=NULL;
	char delim[2]=",";

	int cube_1_length;
	int list_f_in_length;
	int i, j;
	int temp_res_index;
	int temp_memory_size=0;
	int old_res_index;

	if (argc != 3)
	{
		fprintf(stderr, RED"!!! (sharp) illegal arguments\n\x1B[0m sharp <cube 1> {cube 2, cube_3, ..., cube n}\n");
		return TCL_ERROR;
	}

	cube_1 = Tcl_GetStringFromObj(argv[1], &cube_1_length);
	list_f_in = Tcl_GetStringFromObj(argv[2], &list_f_in_length);

	//create_list_of_sharps(&head, cube_1, list_f_in, delim, cube_1_length);
	token = strtok(list_f_in, delim);

	head = (inter_cube_list_T*)malloc(sizeof(inter_cube_list_T));
	if (head == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	head->next= head;
	head->prev= head;

	// get the next token //
	while (token != NULL)
	{
		// remove any whitespace character from cube //
		token = stripwhite(token);
		// check if even and length requirements are met //
		if( do_cube_check(cube_1_length, (int)strlen(token)) == TCL_ERROR)
		{
			return TCL_ERROR;
		}

		curr = (inter_cube_list_T*)malloc(sizeof(inter_cube_list_T));
		if (curr == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			exit(1);
		}	
		curr->next = head->next;
		curr->prev = head;

		head->next = curr;
		head->prev = curr;

		curr->inter_cube_res = (char**) calloc(cube_1_length/2,sizeof(char*));
		if (curr->inter_cube_res == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}

		for (i=0; i<cube_1_length/2; i++)
		{
			curr->inter_cube_res[i] =(char*) calloc(cube_1_length+1,sizeof(char));
			if (curr->inter_cube_res[i] == NULL)
			{
				fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
				return TCL_ERROR;
			}
		}
		do_sharp_2(curr->inter_cube_res, cube_1, token, cube_1_length, (int)strlen(token));

		// find next cube in list //
		token = strtok(NULL, delim);
	}

	// allocate memory size as the size of the result of the oporation between the first 2 #s//
	temp_memory_size = (cube_1_length/2) * (cube_1_length/2);

	temp_result = (char **) calloc(temp_memory_size,sizeof(char*));
	if (temp_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	for (i=0; i<temp_memory_size; i++)
	{
		temp_result[i] = (char *) calloc(cube_1_length+1,sizeof(char));
		if (temp_result[i] == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
	}

	// allocate memory size for the first result //
	old_result = (char **) calloc(cube_1_length/2,sizeof(char*));
	if (old_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	for (i=0; i<cube_1_length/2; i++)
	{
		old_result[i] = (char *) calloc(cube_1_length+1,sizeof(char));
		if (old_result[i] == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
	}

	// initialize the old result with a # f1 in order to start the procedure from the first result //
	// as the results of # operation are inserted in the list in reverse order //
	for (i=0; i<cube_1_length/2; i++)
	{
		if( strcmp(head->prev->inter_cube_res[i],"\0")!=0 )
		{
			old_result[i] = strcpy(old_result[i], head->prev->inter_cube_res[i]);
		}
	}
	// initialize the temp_result index in order to be preserved during the for loops for each node of the list // 
	temp_res_index = 0;
	// initialize the old result iterator limit with the number of the first # //
	old_res_index = cube_1_length/2;
	// continue from the next f2 cube again in reverse order and stop until you reach the head //
	for(curr=head->prev->prev; curr!=head; curr= curr->prev)
	{
		// iterate through the cubes of the old result //
		// the first iteration will go from 0 to cube_1_length //
		for( i=0; i<old_res_index; i++)
		{
			// iterate every a # fn cube result //
			for( j=0; j<cube_1_length/2; j++ )
			{
				// if the cube in question is not empty ( is valid ) and the old result has a valid value //
				// if there is then perform the intersection normally //
				if ( (strcmp(curr->inter_cube_res[j], "\0") != 0) && (strcmp(old_result[i], "\0") != 0) )
				{
					do_cube_intersect(temp_result[temp_res_index], old_result[i], curr->inter_cube_res[j], cube_1_length, cube_1_length);
				}
				// if old result has no valid entry skip //
				else if (strcmp(old_result[i], "\0") == 0)
				{
					break;
				}
				// if cube # result is \0 transfer the result from the old result to temp result  //
				else if ( (strcmp(curr->inter_cube_res[j], "\0") == 0) ) 
				{
					// if the empty record in curr is at the end of the list then transfer cube //
					// from the old result to the temp result and advance index //
					if (j == (cube_1_length/2)-1 )
					{
						temp_result[temp_res_index] = strcpy(temp_result[temp_res_index], old_result[i]);	
						temp_res_index++;
						continue;
					}
					// if not then the next record in curr must be explored //
					else
					{
						continue;
					}
				} 
				// if the resulting cube has 00s in then its is invalid and the memory is released // 
				if ( count_zeros(temp_result[temp_res_index], cube_1_length) != 0 )
				{
					// release memory for invalid entry //
					temp_result[temp_res_index] = (char*)realloc(temp_result[temp_res_index], sizeof(char) );
					if (temp_result[temp_res_index] == NULL)
					{
						fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
						exit(1);
					}
					temp_result[temp_res_index] = "\0";
					continue;
				}
				// increment temp result location in order to keep track between cubes //
				temp_res_index++;
			}
		}
		
		old_result = (char**)realloc(old_result, sizeof(char*)*temp_memory_size );
		if (old_result == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
		for (i=0; i<temp_memory_size; i++)
		{
			old_result[i] = (char *) calloc(cube_1_length+1,sizeof(char));
			if (old_result[i] == NULL)
			{
				fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
				return TCL_ERROR;
			}
		}
		for (i=0; i<temp_memory_size; i++)
		{
			if( strcmp(temp_result[i],"\0")!=0 )
			{
				old_result[i] = strcpy(old_result[i], temp_result[i]);
			}
		}
		// update the iterator limit with the value of data in the temp result //
		old_res_index = temp_memory_size;
		// update memory size to accommodate new & result //
		temp_memory_size = temp_memory_size * (cube_1_length/2);
		//old_result = temp_result;
		
		temp_result = (char**)realloc(temp_result, sizeof(char*)*temp_memory_size );
		if (temp_result == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
		for (i=0; i<temp_memory_size; i++)
		{
			temp_result[i] = (char *) calloc(cube_1_length+1,sizeof(char));
			if (temp_result[i] == NULL)
			{
				fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
				return TCL_ERROR;
			}
		}
		// reset temp_res_index for next node //
		temp_res_index = 0;
		
	}
	// check for duplicates //
	for(i=0; i<old_res_index; i++)
	{ 
		for(j=0; j < old_res_index; j++)
		{
			// skip if j is the same as i then this is the string it self //
			if (j==i)
			{
				continue;
			}
			else
			{
				// if the strings are the same and the string is not NULL then reallocate the memory and erase the entry //
				if( (strcmp(old_result[i], old_result[j]) == 0) && (strcmp(old_result[j],"\0")!=0) )
				{
					old_result[j] = (char*)realloc(old_result[i], sizeof(char) );
					if (old_result[j] == NULL)
					{
						fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
						return TCL_ERROR;
					}
					old_result[j] = "\0";				
				}
			}
		}
	}
	for(curr=head->next; curr!=head; curr= curr->next)
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

	printf("\nFinal Cubes:\n");
	for(i=0; i<old_res_index; i++)
	{
		if( strcmp(old_result[i],"\0")!=0 )
		{
			printf("%s \n", old_result[i]);
		}
	}
	curr= head->next;
	while (curr != head)
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
	free(head);
	for(i=0; i < temp_memory_size; i++)
	{
		if( strcmp(temp_result[i],"\0")!=0 )
		{
			free(temp_result[i]);
		}		
	}
	free(temp_result);

	for(i=0; i < old_res_index; i++)
	{
		if( strcmp(old_result[i],"\0")!=0 )
		{
			free(old_result[i]);
		}		
	}
	free(old_result);	

	return TCL_OK;
}


int off_f(ClientData clientdata, Tcl_Interp *interpreter, int argc, Tcl_Obj *const argv[])
{
	inter_cube_list_T *head=NULL;
	inter_cube_list_T *curr=NULL;
	inter_cube_list_T *curr_temp=NULL;
	char **old_result=NULL;
	char **temp_result=NULL;	
	char *cube_universe=NULL;
	char *list_f_in=NULL;
	char *token=NULL;
	char delim[2]=",";

	int list_f_in_length;
	int universe_length;
	int i, j;
	int temp_res_index;
	int temp_memory_size=0;
	int old_res_index;

	if (argc != 2)
	{
		fprintf(stderr, RED"!!! (off_f) illegal arguments\n\x1B[0m off_f {cube 1, cube_2, ..., cube n}\n");
		return TCL_ERROR;
	}

	list_f_in = Tcl_GetStringFromObj(argv[1], &list_f_in_length);

	//TODO insert check for incorrect delimeter usesage //

	// take the first cube as the size and check all others against this //
	// find the first appearance of delimeter in given list //
	token = strtok(list_f_in, delim);
	// remove any whitespace character from cube //
	token = stripwhite(token);
	universe_length = strlen(token);
	if ( universe_length%2 != 0 )
	{
		fprintf(stderr, RED"!!!(off_f) not even length cube\n\x1B[0m off_f {cube 1, cube_2, ..., cube n}\n");
		return TCL_ERROR;		
	}
	
	cube_universe = calloc(universe_length+1,sizeof(char));
	if (cube_universe == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;		
	}
	for (i=0; i<universe_length; i++)
	{
		strcat(cube_universe, "1");
	}

	head = (inter_cube_list_T*)malloc(sizeof(inter_cube_list_T));
	if (head == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	head->next=head;
	head->prev=head;
	
	while (token != NULL)
	{
		// remove any whitespace character from cube //
		token = stripwhite(token);
		// check if even and length requirements are met //
		if( do_cube_check(universe_length, (int)strlen(token)) == TCL_ERROR)
		{
			return TCL_ERROR;
		}

		curr = (inter_cube_list_T*)malloc(sizeof(inter_cube_list_T));
		if (curr == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}	
		curr->next = head->next;
		curr->prev = head;

		head->next = curr;
		head->prev = curr;

		curr->inter_cube_res = (char**) calloc(universe_length/2,sizeof(char*));
		if (curr->inter_cube_res == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}

		for (i=0; i<universe_length/2; i++)
		{
			curr->inter_cube_res[i] =(char*) calloc(universe_length+1,sizeof(char));
			if (curr->inter_cube_res[i] == NULL)
			{
				fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
				return TCL_ERROR;
			}
		}
		do_sharp_2(curr->inter_cube_res, cube_universe, token, universe_length, (int)strlen(token));

		// find next cube in list //
		token = strtok(NULL, delim);
	}
	// allocate memory size as the size of the result of the oporation between the first 2 #s//
	temp_memory_size = (universe_length/2) * (universe_length/2);

	temp_result = (char **) calloc(temp_memory_size,sizeof(char*));
	if (temp_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	for (i=0; i<temp_memory_size; i++)
	{
		temp_result[i] = (char *) calloc(universe_length+1,sizeof(char));
		if (temp_result[i] == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
	}

	// allocate memory size for the first result //
	old_result = (char **) calloc(universe_length/2,sizeof(char*));
	if (old_result == NULL)
	{
		fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
		return TCL_ERROR;
	}
	for (i=0; i<universe_length/2; i++)
	{
		old_result[i] = (char *) calloc(universe_length+1,sizeof(char));
		if (old_result[i] == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
	}

	// initialize the old result with a # f1 in order to start the procedure from the first result //
	// as the results of # operation are inserted in the list in reverse order //
	for (i=0; i<universe_length/2; i++)
	{
		if( strcmp(head->prev->inter_cube_res[i],"\0")!=0 )
		{
			old_result[i] = strcpy(old_result[i], head->prev->inter_cube_res[i]);
		}
	}
	// initialize the temp_result index in order to be preserved during the for loops for each node of the list // 
	temp_res_index = 0;
	// initialize the old result iterator limit with the number of the first # //
	old_res_index = universe_length/2;
	// continue from the next f2 cube again in reverse order and stop until you reach the head //
	for(curr=head->prev->prev; curr!=head; curr= curr->prev)
	{
		// iterate through the cubes of the old result //
		// the first iteration will go from 0 to universe_length //
		for( i=0; i<old_res_index; i++)
		{
			// iterate every a # fn cube result //
			for( j=0; j<universe_length/2; j++ )
			{
				// if the cube in question is not empty ( is valid ) and the old result has a valid value //
				// if there is then perform the intersection normally //
				if ( (strcmp(curr->inter_cube_res[j], "\0") != 0) && (strcmp(old_result[i], "\0") != 0) )
				{
					do_cube_intersect(temp_result[temp_res_index], old_result[i], curr->inter_cube_res[j], universe_length, universe_length);
				}
				// if old result has no valid entry skip //
				else if (strcmp(old_result[i], "\0") == 0)
				{
					break;
				}
				// if cube # result is \0 transfer the result from the old result to temp result  //
				else if ( (strcmp(curr->inter_cube_res[j], "\0") == 0) ) 
				{
					// if the empty record in curr is at the end of the list then transfer cube //
					// from the old result to the temp result and advance index //
					if (j == (universe_length/2)-1 )
					{
						temp_result[temp_res_index] = strcpy(temp_result[temp_res_index], old_result[i]);	
						temp_res_index++;
						continue;
					}
					// if not then the next record in curr must be explored //
					else
					{
						continue;
					}
				} 
				// if the resulting cube has 00s in then its is invalid and the memory is released // 
				if ( count_zeros(temp_result[temp_res_index], universe_length) != 0 )
				{
					// release memory for invalid entry //
					temp_result[temp_res_index] = (char*)realloc(temp_result[temp_res_index], sizeof(char) );
					if (temp_result[temp_res_index] == NULL)
					{
						fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
						return TCL_ERROR;
					}
					temp_result[temp_res_index] = "\0";
					continue;
				}
				// increment temp result location in order to keep track between cubes //
				temp_res_index++;
			}
		}
		
		old_result = (char**)realloc(old_result, sizeof(char*)*temp_memory_size );
		if (old_result == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
		for (i=0; i<temp_memory_size; i++)
		{
			old_result[i] = (char *) calloc(universe_length+1,sizeof(char));
			if (old_result[i] == NULL)
			{
				fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
				return TCL_ERROR;
			}
		}
		for (i=0; i<temp_memory_size; i++)
		{
			if( strcmp(temp_result[i],"\0")!=0 )
			{
				old_result[i] = strcpy(old_result[i], temp_result[i]);
			}
		}
		// update the iterator limit with the value of data in the temp result //
		old_res_index = temp_memory_size;
		// update memory size to accommodate new & result //
		temp_memory_size = temp_memory_size * (universe_length/2);
		
		temp_result = (char**)realloc(temp_result, sizeof(char*)*temp_memory_size );
		if (temp_result == NULL)
		{
			fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
			return TCL_ERROR;
		}
		for (i=0; i<temp_memory_size; i++)
		{
			temp_result[i] = (char *) calloc(universe_length+1,sizeof(char));
			if (temp_result[i] == NULL)
			{
				fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
				return TCL_ERROR;
			}
		}
		// reset temp_res_index for next node //
		temp_res_index = 0;
		
	}
	// check for duplicates //
	for(i=0; i<old_res_index; i++)
	{ 
		for(j=0; j < old_res_index; j++)
		{
			// skip if j is the same as i then this is the string it self //
			if (j==i)
			{
				continue;
			}
			else
			{
				// if the strings are the same and the string is not NULL then reallocate the memory and erase the entry //
				if( (strcmp(old_result[i], old_result[j]) == 0) && (strcmp(old_result[j],"\0")!=0) )
				{
					old_result[j] = (char*)realloc(old_result[i], sizeof(char) );
					if (old_result[j] == NULL)
					{
						fprintf(stderr, RED"!!!Error in memory allocation \n"NRM);
						return TCL_ERROR;
					}
					old_result[j] = "\0";				
				}
			}
		}
	}
	for(curr=head->next; curr!=head; curr= curr->next)
	{
		printf("Result set :\n");
		for(i=0; i<universe_length/2; i++)
		{
			if( strcmp(curr->inter_cube_res[i],"\0")!=0 )
			{
				printf("%s \n", curr->inter_cube_res[i]);
			}
		}
	}

	printf("\nFinal Cubes:\n");
	for(i=0; i<old_res_index; i++)
	{
		if( strcmp(old_result[i],"\0")!=0 )
		{
			printf("%s \n", old_result[i]);
		}
	}
	curr= head->next;
	while (curr != head)
	{
		for(i=0; i<universe_length/2; i++)
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
	free(head);
	for(i=0; i < temp_memory_size; i++)
	{
		if( strcmp(temp_result[i],"\0")!=0 )
		{
			free(temp_result[i]);
		}		
	}
	free(temp_result);

	for(i=0; i < old_res_index; i++)
	{
		if( strcmp(old_result[i],"\0")!=0 )
		{
			free(old_result[i]);
		}		
	}
	free(old_result);	

	return TCL_OK;
}

