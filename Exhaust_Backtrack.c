/*
 * Exhaust_Backtrack.c
 *
 *  Created on: 27 באוג׳ 2019
 *      Author: ליהי
 */


#include "Exhaust_Backtrack.h"



/*Put in res_col and res_row the next cell on the board, going from left to right and top to bottom*/
/*if the last cell is reached, return 0 and 1 otherwise*/
int next_cell(Board *board, int col, int row, int *res_col, int *res_row){
	int board_size = board->m * board->n;

	if(col==board_size && row == board_size){
		*res_col=0;
		*res_row=0;
		return 0;
	}
	else if(col!=board_size){
		*res_col=col+1;
		*res_row=row;
	}
	else{
		*res_col=1;
		*res_row=row+1;
	}
	return 1;
}




/*Put in res_col and res_row the next cell that is empty, going from left to right and top to bottom*/
/*if there is no such cell, return 0 and 1 otherwise*/
int next_empty(Board *board, int col, int row, int *res_col, int *res_row){
	int next;
	next = next_cell(board, col, row, res_col, res_row);
	while(next!=0){
		if(is_empty_cell(board, *res_col, *res_row)){
			return 1;
		}

		else{
			col = *res_col;
			row = *res_row;
		}
		next = next_cell(board, *res_col, *res_row, res_col, res_row);
	}
	return 0;
}

/*Insert to the stack all the possible legal assignments to cell (col, row)*/
/*return the number of elements inserted*/
int push_options(Board *board, int col, int row, Stack *stk){
	int board_size = board->m * board->n;
	int cnt=0;
	int i;
	for(i=board_size; i>=1; i--){
		if(is_legal_set(board, col, row, i)){
			move *opt = (move*)malloc(sizeof(move));
			if (opt == NULL) {memory_error("malloc");}
			opt->col = col;
			opt->row = row;
			opt->value = i;
			push(stk, opt);
			cnt++;
		}
	}
	return cnt;
}


/*Delete any value on the board that is in a cell beyond cell (col, row), going from left to right and top to bottom*/
void clean_after_cell(Board *board, int col, int row){
	int i, j, next;
	next=next_cell(board, col, row, &j, &i);
	while(next!=0){
		if(!(is_fixed_cell(board, j, i))){
				clean_cell(board, j, i);
		}
		next = next_cell(board, j, i, &j, &i);
	}
}

/*Fix all the non-empty cells currently on the board*/
void fix_all(Board *board){
	int i, j;
	int BOARD_SIZE = board->m * board->n;
	for(i=1; i<=BOARD_SIZE; i++){
		for (j=1; j<=BOARD_SIZE; j++){
			if(!(is_empty_cell(board, j, i))){
				fix_cell(board, j, i);
			}
		}
	}
}



/*Take a possible assignment out of the stack and try to place it on board and "go back" or keep moving forword on the board*/
/*return 1 iff the operation succeeded*/
int put_value(Board *board, Stack *stk, int *col, int *row, int backward){
	move *opt;
	opt = pop(stk);
	if(is_empty(stk)){
		return 0;
	}

	set_cell(board, opt->col, opt->row, opt->value);
	if(backward){
		clean_after_cell(board, opt->col, opt->row);
		*col = opt->col;
		*row = opt->row;
	}
	free(opt);
	return 1;
}

/*take a step in the backwoard direction of the backtraking. This is the same as going
 * forword, but here we also need to clean all the cells that are beyond the cell to be put
 * (which was chosen by popping and element from the stack*/

void backward(Board *board, Stack *stk, int *col, int *row){
	put_value(board, stk, col, row, 1);
}

void forward(Board *board, Stack *stk, int *col, int *row){
	put_value(board, stk, col, row, 0);
}



/*(Described in the header file)*/
int count_solutions(Board *orig){
	int col=0, row=1, next, cnt=0;
	Stack *stk;
	move *bottom;
	Board *board;
	stk = (Stack*)malloc(sizeof(Stack));
	if (stk == NULL) {memory_error("malloc");}
	bottom= (move*)malloc(sizeof(move));
	if (bottom == NULL) {memory_error("malloc");}
	board = get_board_copy(orig);
	fix_all(board);
	init_Stack(stk);
	push(stk, bottom);

	do{
		next = next_empty(board, col, row, &col, &row);
		if(next == 0 ){/*board is full*/
			cnt++;
		}
		else{/*try to go forward*/
			push_options(board, col, row, stk);
		}
		backward(board, stk, &col, &row);
	}while(!is_empty(stk));

	destroy_Stack(stk);
	destroy_Board(board);
	free(bottom);
	return cnt;
}



