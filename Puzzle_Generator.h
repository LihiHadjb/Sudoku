/*
 * Puzzle_Generator.h
 *
 *  Created on: 16 בספט׳ 2019
 *      Author: ליהי
 */

#ifndef PUZZLE_GENERATOR_H_
#define PUZZLE_GENERATOR_H_


#include "general.h"
#include "Board.h"
#include "GRB_Solver.h"
#include "Memory.h"

#define NUM_ITERS 1000
int generate_board(Board *board, Board **res_board, int x, int y);
void add_generate_changes_to_mem(Memory *mem, Board *res_board, Board *org_board);



#endif /* PUZZLE_GENERATOR_H_ */
