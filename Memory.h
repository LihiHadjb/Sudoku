/*
 * Memory.h
 *
 *  Created on: Sep 14, 2019
 *      Author: DorAlon
 */

#ifndef MEMORY_H_
#define MEMORY_H_


#include "general.h"
#include "SPBufferset.h"


typedef struct node {
    int col,row,newVal,oldVal;
    struct node *next;
} node;

typedef struct list {
	int size,id;
	node *top;
	struct list *next;
	struct list *prev;
    int undoed;
    int redoed;
} list;


typedef struct Memory {
	int size;
	list *currentPointer;
	list *top;
} Memory;
void destroy_mem(Memory *mem);
node* create_node(int col, int row, int newVal, int oldVal);
Memory* init_mem();
void destroy_memory(Memory *mem);
list* create_list();
void push_mem(Memory *mem, list *newList);
void push_to_list(list *list, node *pointer);
void push_to_last_list(Memory *mem, node *pointer);
list* pop_mem(Memory *mem);
int is_empty_mem(Memory *mem);
void update_list(list *list, int x, int y, int z, int oldVals);
void clean_redo(Memory *mem);
#endif /* MEMORY_H_ */
