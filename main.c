#include <stdio.h>
#include <stdlib.h>

#include "tcl/tcl_functions.h"
#include "readline/readline_functions.h"

int main(int argc, char *argv[])
{
	
	print();

	find_executable(argv[0]);

	init_interpreter();

    return 0;
}
