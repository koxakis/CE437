CC=gcc
CFLAGS= -Wall -g -std=c11 -I.
DEPS= tcl/cl_functions.h
OBJ= main.o tcl/tcl_functions.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main_game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean_obj:
	rm $(OBJ) 

clean_all:
	rm $(OBJ) main