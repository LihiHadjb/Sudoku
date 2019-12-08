/*
 * general.h
 *
 *This module functions and indcldes for general use by all the other modules
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPBufferset.h"
#include <ctype.h>

/*notify the user that a memory allocation failed and terminate the program*/
void memory_error(const char* func_name);

/*initialize the memory allocated to str to get ridd of garbage values*/
void init_str_mem(char *str, int size);

#endif /* GENERAL_H_ */
