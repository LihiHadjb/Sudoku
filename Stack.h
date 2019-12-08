/*
 * Stack.h
 *
 * This module is an implementation for a stack, used by the Exhaustive Backtracking module to replace
 * recursive calls.
 * Each element of the stack represents a legal assignment for cell col, row if value val.
 */

#ifndef STACK_H_
#define STACK_H_

#include "general.h"


typedef struct move{
	int col;
	int row;
	int value;
	struct move* next;
}move;



typedef struct Stack{
	int size;
	move *top;
}Stack;
/*return 1 iff stk contains no elements*/
int is_empty(Stack *stk);

/*pop an element from stk and return it*/
/*assumes stack is not empty*/
move* pop(Stack *stk);

/*insert the element opt to stk*/
void push(Stack *stk, move *opt);

/*initialize stk*/
void init_Stack(Stack *stk);

/*free all memory resources related to stk*/
void destroy_Stack(Stack *stk);

#endif /* STACK_H_ */
