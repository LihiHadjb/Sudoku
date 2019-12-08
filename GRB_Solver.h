/*
 * GRB_Solver.h
 *
 *  This module is responsible for determining whether a given board is solvable, and to find such
 *  a solutions in the ILP case or giving scores to every possible assignment to every empty cell
 *  is the LP case. It does this by using the Options_Board struc, which enables us to build the
 *  constraints according to the board efficiently.
 */

#ifndef GRB_SOLVER_H_
#define GRB_SOLVER_H_


#include "Options_Board.h"
#include "general.h"
#include "Messages.h"
#include "gurobi_c.h"
#include "Solver_Helpers.h"



#define RANGE_B 4
#define RANGE_T 5

#define LP 0
#define ILP 1


/*create a Gurobi model and add to it variables and constraints according to the current board
 * state. opts_board is a pointer to an Options_Board object that may be used later by
 * the caller, and sol is a pointer to an array in which the solutions should be stored, also for
 * later use by the caller.
 * return 1 iff solving the model succeeded (assumes the board is not erroneous)
 */

int call_GRB(Board *board, double **sol, int is_ILP, Options_Board *opts_board);





#endif /* GRB_SOLVER_H_ */
