/*
 * Random.h
 *
 * This module is contains helpers for the Puzzle_Generator module*/


#ifndef RANDOM_H_
#define RANDOM_H_
#include <time.h>
#include "general.h"
#include <math.h>


/*return a random index in the range 0 to size with probalbilities probs for each index*/
int get_rand_index_with_probs(double *probs, int size);

#endif /* RANDOM_H_ */
