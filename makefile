CC=gcc
CFLAGS= -Wall -g -I. -ltcl8.6 -lreadline
OBJ= main.o file_functions/parser.o tcl/tcl_functions.o readline/readline_functions.o 

tclShell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# readline function make 
readline/readline_functions.o: readline/readline_functions.c readline/readline_functions.h
	$(CC) -c -o $@ $< $(CFLAGS)

# tcl function make 
file_functions/parser.o: file_functions/parser.c file_functions/parser.h
	$(CC) -c -o $@ $< $(CFLAGS)

# tcl function make 
tcl/tcl_functions.o: tcl/tcl_functions.c tcl/tcl_functions.h
	$(CC) -c -o $@ $< $(CFLAGS)

# clean
clean:
	rm $(OBJ) tclShell
