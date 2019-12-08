/*
 * Options_Board.c
 *
 *  Created on: 7 בספט׳ 2019
 *      Author: ליהי
 */
#include "Options_Board.h"

Options_Board* create_Options_Board(int m, int n){
	int i, j;
	int board_size = m*n;
	Options_Board *opts_board = (Options_Board*) malloc(sizeof(Options_Board));
	if(opts_board == NULL) {memory_error("malloc");}
	opts_board->num_elems = 0;
	opts_board->board = (CO_queue***)malloc(board_size * sizeof(CO_queue**));
	if(opts_board->board == NULL) {memory_error("malloc");}
	for (i = 0; i < board_size; i++) {
		opts_board->board[i] = (CO_queue**) malloc(board_size * sizeof(CO_queue*));
		if(opts_board->board[i] == NULL) {memory_error("malloc");}
		for(j=0; j<board_size; j++){
			opts_board->board[i][j] = create_CO_queue();
		}
	}
	opts_board->m = m;
	opts_board->n = n;
	return opts_board;
}





void init_Options_Board(Board *board, Options_Board *opts_board, int board_size){
	int i, j;
	for (i=1; i<=board_size; i++){
		for (j=1; j<=board_size; j++){
			if(is_empty_cell(board, j, i)){
				add_legal_values(board, j, i, opts_board);
			}
		}
	}
}


void destroy_Options_Board(Options_Board *opts_board){
	int i, j;
	int board_size = opts_board->m * opts_board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			destroy_CO_queue(CO_get_cell(opts_board, j, i));
		}
		free(opts_board->board[i-1]);
	}
	free(opts_board->board);
	free(opts_board);
}

void add_elem_to_board(Options_Board *opts_board, int col, int row ,Option *elem){
	CO_queue *q;
	q = CO_get_cell(opts_board, col, row);
	insert(q, elem);
	opts_board->num_elems++;
}

void add_to_board(Options_Board *opts_board, int col, int row ,int val){
	Option *elem;
	elem = create_Option(opts_board->num_elems, val, NULL);
	add_elem_to_board(opts_board, col, row, elem);

}

Option* remove_first_from_board(Options_Board *opts_board, int col, int row){
	Option *first = dequeue(CO_get_cell(opts_board, col, row));
	opts_board->num_elems--;
	return first;
}


void add_legal_values(Board *board, int col, int row, Options_Board *opts_board){
	int i;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		if(is_legal_set(board, col, row, i)){
			add_to_board(opts_board, col, row ,i);
		}
	}
}




void CO_set_cell(Options_Board *board, CO_queue *q, int col, int row){
	board->board[col-1][row-1] = q;
	board->num_elems+=q->size;
}

CO_queue *CO_get_cell(Options_Board *board, int col, int row){
	return board->board[col-1][row-1];
}

int CO_get_list_size(Options_Board *board, int col, int row){
	return board->board[col-1][row-1]->size;
}







/*return the number of cells in block with index index that val is a legal assignment for according to cells_lists
 * (since the elements are inserted in ascending order, it is sufficient to check only the first
 * Element in each queue on the block*/

int cnt_num_occurs_block(Options_Board *cells_lists, int val, int index, int m, int n){
	int i,j, cnt=0;
	int first_row, first_col;
	first_col_row_in_block(m, n, index, &first_col, &first_row);
	for(i=0; i<m; i++){
		for(j=0; j<n; j++){
			if(!is_empty_Q(CO_get_cell(cells_lists, first_col+j, first_row+i)) && peek_value(cells_lists,first_col+j, first_row+i)==val){
					cnt++;
			}
		}
	}
	return cnt;

}



/*return the number of cells in colomn col that val is a legal assignment for according to cells_lists
 * (since the elements are inserted in ascending order, it is sufficient to check only the first
 * Element in each queue on the block*/
int cnt_num_occurs_col(Options_Board *cells_lists, int val, int col, int board_size){
	int cnt = 0, row;
	for(row=1; row<=board_size; row++){
		if(!is_empty_Q(CO_get_cell(cells_lists, col, row)) && peek_value(cells_lists,col, row)==val){
			cnt++;
		}
	}
	return cnt;
}



/*return the number of cells in row row that val is a legal assignment for according to cells_lists
 * (since the elements are inserted in ascending order, it is sufficient to check only the first
 * Element in each queue on the block*/

int cnt_num_occurs_row(Options_Board *cells_lists, int val, int row, int board_size){
	int cnt = 0, col;
	for(col=1; col<=board_size; col++){
		if(!is_empty_Q(CO_get_cell(cells_lists, col, row)) && peek_value(cells_lists,col, row)==val){
			cnt++;
		}
	}
	return cnt;
}


int cnt_num_occurs(Options_Board *cells_lists, int val, int index, int type, int m, int n) {
	int board_size = m*n;
	switch(type){
	case(ROW):
		return cnt_num_occurs_row(cells_lists, val, index, board_size);
	case(COL):
		return cnt_num_occurs_col(cells_lists, val, index, board_size);
	case(BLOCK):
		return cnt_num_occurs_block(cells_lists, val, index, m, n);
	}
	return -1;
}








int peek_value(Options_Board *board, int col, int row){
	return CO_get_cell(board, col, row)->first->value;
}

int peek_index(Options_Board *board, int col, int row){
	return CO_get_cell(board, col, row)->first->idx;
}

void fill_obvious_cells(Board *board, Memory *mem){
	int m = board->m;
	int n = board->n;
	int board_size = m*n;
	int i, j;
	list *step=create_list();
	Options_Board *opts = create_Options_Board(m, n);
	init_Options_Board(board, opts, board_size);
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			if(CO_get_list_size(opts, j, i) == 1){
				update_mem(get_cell(board, j, i), j, i, peek_value(opts, j, i), step);
				set_cell(board, j, i, peek_value(opts, j, i));
			}
		}
	}
	push_mem(mem, step);
	destroy_Options_Board(opts);
}



void first_col_row_in_block(int m, int n, int block_idx, int *res_col, int *res_row){
	int block_col, block_row;
	block_col = (block_idx-1) % m + 1;
	block_row = (int)((block_idx-1) / m) + 1;
	*res_row = (block_row-1) * m + 1;
	*res_col = (block_col-1) * n + 1;
}


