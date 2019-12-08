/*
This module implements the state of the game board. We decided to create a separate module for the board,
so that the Game and Solver modules can work independently of the inner representation of the
board structure and cells.
For example, if one needs to change the representation of the board to a
2D array of strings (a digit with a dot for a fixed cell and a digit for a non-fixed cell), it is possible
to so by updating this module without having to change the game commands code (set, hint etc...).
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "general.h"
#include "Memory.h"


/*Each board is represented as two 2D arrays,
 * one for the values in the cells and the other contains binary values that indicate if the cell is fixed.
 */
typedef struct Board{
	int** numbers;
	int** fixed_cells;
	/*struct Cell_options *cell_options;*/
	int n;
	int m;
}Board;


/*Print the current state of the board to user*/
void print_board(Board *board, int mark_errors);

/*Place num_to_set on cell (col,row)*/
void set_cell(Board *board, int x, int y, int num_to_set);

/*Get the current value in cell (col, row)*/
int get_cell(Board *board, int x, int y);

/*Return 1 if cell (x, y) in currently fixed and 0 otherwise*/
int is_fixed_cell(Board *board, int x, int y);

/*Return 1 if cell (x, y) is currently empty*/
int is_empty_cell(Board *board, int x, int y);

/*Create a board instance*/
Board* create_Board();

/*Free all memory related to the board*/
void destroy_Board(Board *board);


/*Make cell (col, row) fixed*/
void fix_cell(Board *board, int x, int y);

/*Make cell (col, row) not fixed*/
void unfix_cell(Board *board, int x, int y);

/*Delete any value currently in cell (col, row)*/
void clean_cell(Board *board, int x, int y);

/*Check whether setting cell (x,y) to num_to_set is an error (iff (x,y) has a neighbor with the same value)*/
int is_legal_set(struct Board *board, int x, int y, int num_to_set);

/*Get a "deep clone" of the board*/
Board* get_board_copy(Board *board);

/*Check whether the board contains errors (i.e. neighbors set to identical values)*/
int is_erroneous(Board *board);

/*Check if the board is full*/
int is_finished(Board *board);

/*Return the number of cells with no values in them*/
int get_num_empty_cells(Board *board);




/*Set all the non-empty cells on the board to be empty*/
void fix_all_non_empty(Board *board);
void unfix_all(Board *board);
void update_mem( int oldVal, int col, int row, int num_to_set, list *step);



#endif /* BOARD_H_ */
