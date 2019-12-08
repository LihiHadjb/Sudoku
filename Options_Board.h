/*
 * Options_Board.h
 *This module represents a struct that saves the lagal assignment of each empty cell on a given
 *board.
 *An Options_Board instance consists of a 2D array of the CO_queue struct (descibed in detail in
 * relevant h file).
 *
 *
 */

#ifndef OPTIONS_BOARD_H_
#define OPTIONS_BOARD_H_



#include "CO_queue.h"


#define ROW 0
#define COL 1
#define BLOCK 2
#define CELL 3



typedef struct Options_Board{
	int num_elems;
	struct CO_queue ***board;
	int m;
	int n;
}Options_Board;


/*create and return an instance of Options_Board with dims m, n. The object is initialized to empty lists
 */
Options_Board* create_Options_Board(int m, int n);

/*fill the lists of each empty cell on opts_board of  the board with its legal values*/
void init_Options_Board(Board *board, Options_Board *opts_board, int board_size);

/*Free all memory resources related to opts_board*/
void destroy_Options_Board(Options_Board *opts_board);

/*dequeue and return an element from the queue of cell col,row on opts_board*/
Option* remove_first_from_board(Options_Board *opts_board, int col, int row);

/*add an element to the queue in cell col,row of opts_board, with value val*/
void add_to_board(Options_Board *opts_board, int col, int row ,int val);

/*Add the Options object elem to the queue of cell col, row in opts_board*/
void add_elem_to_board(Options_Board *opts_board, int col, int row ,Option *elem);

/*Add to the queue of cell col,row in opts_board all the legal assignmnets of cell col, row according
 * to the current state of board
 */
void add_legal_values(Board *board, int col, int row, Options_Board *opts_board);


/*a switch for the 3 methods: cnt_num_occurs_row, cnt_num_occurs_col, cnt_num_occurs_block, chosen
 * according to type*/
int cnt_num_occurs(Options_Board *cells_lists, int val, int index, int type, int m, int n);


/*Sets the list of cell col,row in  board to be q*/
void CO_set_cell(Options_Board *board, CO_queue *q, int col, int row);

/*return the list of legal values for cell col,row in  board*/
CO_queue *CO_get_cell(Options_Board *board, int col, int row);

/*return the number of elements in the lits of legal values for cell col,row in board*/
int CO_get_list_size(Options_Board *board, int col, int row);

/*return the value of the first element of the queue of cell col,row in board*/
int peek_value(Options_Board *board, int col, int row);

/*return the index of the first element of the queue of cell col,row in board*/
int peek_index(Options_Board *board, int col, int row);


/*fill cells  on board that have only one legal value with this value, and update the actions done
 * in mem
 */
void fill_obvious_cells(Board *board, Memory *mem);

/*puts is res_col, res_row that index of the first colomn and row in block with index index of a board
 * with dims m,n. Blocks are counted left to right and top to bottom
 */
void first_col_row_in_block(int m, int n, int block_idx, int *res_col, int *res_row);



#endif /* OPTIONS_BOARD_H_ */
