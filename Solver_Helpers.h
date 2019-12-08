/*
 * Solver_Helpers.h
 *
This module includes auxiliary functions for extracting the solutions out of the array returned by
Gurobi (used by hint, guess_hint and guess) and helpers for bulding the model's constraints
 */

#ifndef SOLVER_HELPERS_H_
#define SOLVER_HELPERS_H_
#include "Options_Board.h"
#include "general.h"



/*initialize the sol array to clean garbage values*/
void init_sol(int num_vars,double **sol);


/*return the number of values that got a score>threshold for cell col,row, using the solution indices in score that are "translated" to values using opts_board*/
int cnt_num_greater_than_threshold(double **sol, Options_Board *opts_board, int col, int row, double threshold);

/*fill the values in the board according to the solution indices in score that are
 * "translated" to values using opts_board*/
void fill_solution(double **sol, Board *solution_board, Options_Board *opts_board);

/*put in scores the resulting scores for cell col, row, greater that threshold, and according to the indices in opts_board*/
/*for guess_hint use threshold=0*/
int get_scores_for_cell(Options_Board *opts_board,double **sol, double **scores, int **values, int col, int row, double threshold);

/*extract and return the solution for cell col,row stored in sol. this is done by finding the value
 * of index of that variable in opts_board*/
int get_sol_for_cell(Options_Board *opts_board, double **sol, int col, int row);


/*return 1 iff there is a cell in block block_idx with value val on board*/
int in_block(Board *board, int block_idx, int val);

/*return 1 iff there is a cell in col with value val on board*/
int in_col(Board *board, int col, int val);

/*return 1 iff there is a cell in row with value val on board*/
int in_row(Board *board, int row, int val);

#endif /* SOLVER_HELPERS_H_ */
