/*
 * Solver_Helpers.c
 *
 *  Created on: 19 בספט׳ 2019
 *      Author: ליהי
 */

#include "Solver_Helpers.h"




void init_sol(int num_vars,double **sol){
	int i;
	*sol = (double*)malloc(sizeof(double) * (num_vars));
	if(sol == NULL) {memory_error("malloc");}
	for(i=0; i<num_vars; i++){
		(*sol)[i] = 0;
	}
}




int in_row(Board *board, int row, int val){
	int j;
	int board_size = board->m * board->n;
	for(j=1; j<=board_size; j++){
		if(get_cell(board, j, row) == val){
			return 1;
		}
	}
	return 0;
}


int in_col(Board *board, int col, int val){
	int i;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		if(get_cell(board, col, i) == val){
			return 1;
		}
	}
	return 0;
}


int in_block(Board *board, int block_idx, int val){
	int i,k, cell_to_check, check_col, check_row;
	int first_col_in_block, first_row_in_block;
	first_col_row_in_block(board->m, board->n, block_idx, &first_col_in_block, &first_row_in_block);
	for (i = 0; i < board->m ; i++) {
		for (k = 0; k < board->n; k++) {
			check_col = first_col_in_block+k;
			check_row = first_row_in_block+i;
			cell_to_check = get_cell(board, check_col, check_row);
			if (cell_to_check == val) {
				return 1;
			}
		}
	}
	return 0;
}





int cnt_num_greater_than_threshold(double **sol, Options_Board *opts_board, int col, int row, double threshold){
	int i, cnt=0, var_idx;
	Option *elem;
	for(i=1; i<=CO_get_list_size(opts_board, col, row); i++){
		var_idx = peek_index(opts_board, col, row);
		elem = remove_first_from_board(opts_board, col, row);
		add_elem_to_board(opts_board, col, row, elem);
		if((*sol)[var_idx]>=threshold){
			cnt++;
		}
	}
	return cnt;
}




void fill_solution(double **sol, Board *solution_board, Options_Board *opts_board){
	int board_size = solution_board->m * solution_board->n;
	int i, j,k, size, ind;
	Option *elem;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			size = CO_get_list_size(opts_board, j, i);
			for(k=1; k<=size; k++){
				ind = peek_index(opts_board, j, i);
				elem = remove_first_from_board(opts_board, j, i);
				if((*sol)[ind] == 1){
					set_cell(solution_board, j, i, elem->value);
				}
				destroy_Option(elem);
			}
		}
	}
}





int get_scores_for_cell(Options_Board *opts_board,double **sol, double **scores, int **values, int col, int row, double threshold){
	int size;
	int i,curr=0, var_idx, var_val;
	Option *elem;

	size = cnt_num_greater_than_threshold(sol, opts_board, col, row, threshold);
	*scores = (double*)malloc(sizeof(double)*size);/*freed by caller*/
	if(scores == NULL) {memory_error("malloc");}
	*values = (int*)malloc(sizeof(int)*size);/*freed by caller*/
	if(values == NULL) {memory_error("malloc");}
	for(i=0; i<size; i++){
		var_idx = peek_index(opts_board, col, row);
		var_val = peek_value(opts_board, col, row);
		elem = remove_first_from_board(opts_board, col, row);
		add_elem_to_board(opts_board, col, row, elem);
		if((*sol)[var_idx]>=threshold){
			(*scores)[curr] = (*sol)[var_idx];
			(*values)[curr] = var_val;
			curr++;
		}
	}
	return curr;
}




int get_sol_for_cell(Options_Board *opts_board, double **sol, int col, int row){
	int size, k, ind, val;
	Option *elem;
	size = CO_get_list_size(opts_board, col, row);
	for(k=1; k<=size; k++){
		ind = peek_index(opts_board, col, row);
		val = peek_value(opts_board, col, row);
		elem = remove_first_from_board(opts_board, col, row);
		add_elem_to_board(opts_board, col, row, elem);
		if((*sol)[ind] == 1){
			return val;
		}

	}
	return 0;
}
