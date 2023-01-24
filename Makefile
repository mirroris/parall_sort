MAIN = MSD_radix.c
OBJS = main.o zemib.o
PROGRAM = parallel
OPTION = -fopenmp

default: zemib.o main.o
	@gcc -o $(PROGRAM) $(OPTION) zemib.o main.o

debug: $(MAIN) zemib.c
	@gcc -g -c -o zemib.dbg zemib.c 
	@gcc -g -c -o main.dbg $(MAIN) 
	@gcc -g -o $(PROGRAM).dbg $(OPTION) zemib.dbg main.dbg

zemib.o: zemib.c
	@gcc -c -o zemib.o zemib.c

main.o: $(MAIN)
	@gcc -c -o main.o $(MAIN)

clean:
	@rm -f *.o $(PROGRAM) *.dbg