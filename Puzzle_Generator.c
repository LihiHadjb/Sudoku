/*
 * Puzzle_Generator.c
 *
 *  Created on: 16 בספט׳ 2019
 *      Author: ליהי
 */
#include "Puzzle_Generator.h"


/*put in res_col and res_col random values in the range 1-board_size*/
void choose_random_cell(Board *board,  int *res_col, int *res_row){
	int board_size = board->m * board->n;
	*res_col = rand()%board_size + 1;
	*res_row = rand()%board_size + 1;
}


/*put in res_col and res_row the coordinates of a random empty cell on the given board*/
/*assumes there is at least one empty cell*/
void choose_random_empty_cell(Board *board, int *res_col, int *res_row){
	int found=0;
	while(!found){
		choose_random_cell(board, res_col, res_row);
		if(is_empty_cell(board, *res_col, *res_row)){
			found = 1;
		}
	}
}


/*retrun a legal value for cell (col, row) that is the i'th in ascending order among all the lega
 * values for this cell on board
 */
int get_ith_legal_value(Board *board, int index, int col, int row){
	int val=0, cnt=0;
	while(cnt<=index){
		val++;
		if(is_legal_set(board, col, row, val)){
			cnt++;
		}
	}
	return val;

}



/*returns a random legal assignment for cell (col, row) */
int get_random_value_from_legals(Board *board_copy, int num_legals, int col, int row){
	int index, val;
	index = rand() % num_legals;
	val = get_ith_legal_value(board_copy, index, col, row);
	return val;
}


/*returns the number of legal values to assign on cell(col, row)*/
int get_num_legals(Board *board_copy, int col, int row){
	int val, cnt=0;
	int board_size = board_copy->m * board_copy->n;
	for(val=1; val<=board_size; val++){
		if(is_legal_set(board_copy, col, row, val)){
				cnt++;
		}
	}

	return cnt;
}


/*fill x random empty cells of board_copy with random legal values. If at some point the chosen cell does not
 * have any legal assignment, reuturn 0, otherwise return 1*/
int fill_x_empty_cells(Board *board_copy, int x){
	int i, val, col=0, row=0, found=0, num_legals;
	/*int board_size = board_copy->m * board_copy->n;*/

	/*Options_Board *opts = create_Options_Board(board_copy->m, board_copy->n);
	init_Options_Board(board_copy, opts, board_size);*/
	for(i=0; i<x; i++){
		found = 0;
		choose_random_empty_cell(board_copy, &col, &row);
		if((num_legals=get_num_legals(board_copy, col, row)) == 0){
			/*destroy_Options_Board(opts);*/
			return 0;
		}
		while(!found){
			val = get_random_value_from_legals(board_copy, num_legals, col, row);
			if(is_legal_set(board_copy, col, row, val)){
				found = 1;
				set_cell(board_copy, col, row, val);
			}
		}
	}
	/*destroy_Options_Board(opts);*/
	return 1;
}

/*choose random y cells to keep and clear all the other cells on board*/
void clean_all_but_y_cells(Board *board, int y){
	int col, row;
	int board_size = board->m * board->n;
	int num_cells = board_size * board_size;
	while(get_num_empty_cells(board) < num_cells-y){
		choose_random_cell(board, &col, &row);
		clean_cell(board, col, row);
	}
}




/*Attempt to generates a puzzle by filling x empty cells, solving the resulting board and then cleaning all
 * but y cells of it, within NUM_ITERS(1000) iterations. Return the resulting board if generation
 * Succeeded and NULL otherwise*/

int generate_board(Board *board, Board **res_board, int x, int y){
	int k;
	int m = board->m;
	int n = board->n;

	/*Board *temp;*/
	Board *board_copy;
	double **sol = (double**)malloc(sizeof(double));
	if(sol == NULL) {memory_error("malloc");}



	for(k=1; k<=NUM_ITERS; k++){
			Options_Board *opts;
			board_copy = get_board_copy(board);
			opts = create_Options_Board(m,n);
			/*try to fill x cells*/
			if(!fill_x_empty_cells(board_copy, x)){
				destroy_Board(board_copy);
				destroy_Options_Board(opts);
				continue;
			}

			/*try to solve the board*/
			if(!call_GRB(board_copy, sol, ILP, opts)){
				destroy_Board(board_copy);
				destroy_Options_Board(opts);
				continue;
			}


			/*generation succeeded*/
			fill_solution(sol, board_copy, opts);
			clean_all_but_y_cells(board_copy, y);
			*res_board = board_copy;
			destroy_Options_Board(opts);
			return 1;
	}

	/*generation failed*/
	return 0;
}


/*update the Memory object in mem according to the cells that were updated during the generation
 * of the board res_board with respect to org_board*/

void add_generate_changes_to_mem(Memory *mem, Board *res_board, Board *org_board){
	list *gen_step = create_list();
	int i, j, old;
	int board_size =org_board->m * org_board->n;
		for(i=1; i<=board_size; i++){
			for(j=1; j<=board_size; j++){
					if(get_cell(org_board, j, i)!= get_cell(res_board,j, i)){
						old = get_cell(org_board, j, i);
						update_mem(old, j, i, get_cell(org_board, j, i), gen_step);
					}


			}
		}
		push_mem(mem, gen_step);
}

