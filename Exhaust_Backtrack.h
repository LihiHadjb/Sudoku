/*
 * Exhaust_Backtrack.h
 *
 *This module is an implementation of the Exhaustive Backtracking algorithm,
 *using an explicit stack instead of recursion*/



#ifndef EXHAUST_BACKTRACK_H_
#define EXHAUST_BACKTRACK_H_

#include "general.h"
#include "Board.h"
#include "Stack.h"

/*Return the number of solutions the board has, using the backtrack algorithm with a stack*/
int count_solutions(Board *board);



#endif /* EXHAUST_BACKTRACK_H_ */
