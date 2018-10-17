CC=gcc
CFLAGS= -Wall -g -I. -ltcl8.6 -lreadline
OBJ= main.o tcl/tcl_functions.o readline/readline_functions.o

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# readline function make 
readline/readline_functions.o: readline/readline_functions.c readline/readline_functions.h
	$(CC) -c -o $@ $< $(CFLAGS)

# tcl function make 
tcl/tcl_functions.o: tcl/tcl_functions.c tcl/tcl_functions.h
	$(CC) -c -o $@ $< $(CFLAGS)

# clean
clean_obj:
	rm $(OBJ) 

clean_all:
	rm $(OBJ) main