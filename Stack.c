/*
 * Stack.c
 *
 *  Created on: 27 באוג׳ 2019
 *      Author: ליהי
 */


#include "Stack.h"

void init_Stack(Stack *stk){
	stk->size=0;
	stk->top=NULL;
}

void destroy_Stack(Stack *stk){
	while(!is_empty(stk)){
		move *elem;
		elem = pop(stk);
		free(elem);
	}
	free(stk);
}

void push(Stack *stk, move *opt){
	opt->next = stk->top;
	stk->top = opt;
	stk->size++;
}


move* pop(Stack *stk){
	move *result;
	result = stk->top;
	stk->top = stk->top->next;
	stk->size--;
	return result;
}

int is_empty(Stack *stk){
	return stk->top == NULL;
}

