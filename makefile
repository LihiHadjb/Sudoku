CC = gcc
OBJS = main.o general.o Messages.o Parser.o Files.o Board.o Memory.o Puzzle_Generator.o Game.o CO_queue.o Options_Board.o Random.o Solver_Helpers.o GRB_Solver.o Stack.o Exhaust_Backtrack.o
  
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors -g -O0
COMP_FLAGS_GUR = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
	
main.o: main.c SPBufferset.h Board.h GRB_Solver.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
general.o: general.c general.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Messages.o: Messages.c Messages.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Files.o: Files.c Files.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Board.o: Board.c Board.h general.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Memory.o: Memory.c Memory.h 
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Puzzle_Generator.o: Puzzle_Generator.c Puzzle_Generator.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
CO_queue.o: CO_queue.c CO_queue.h Board.h general.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Options_Board.o: Options_Board.c Options_Board.h CO_queue.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Random.o: Random.c Random.h 
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Solver_Helpers.o: Solver_Helpers.c Solver_Helpers.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
GRB_Solver.o: GRB_Solver.c GRB_Solver.h Options_Board.h general.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Stack.o: Stack.c Stack.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c
Exhaust_Backtrack.o: Exhaust_Backtrack.c Exhaust_Backtrack.h
	$(CC) $(COMP_FLAGS_GUR) $(GUROBI_COMP) -c $*.c


clean:
	rm -f $(OBJS) $(EXEC)