/*
 * memmm.c
 *
 *  Created on: 20 בספט׳ 2019
 *      Author: ליהי
 */
#include "Memory.h"



Memory* init_mem() {
	Memory *mem=(Memory*)malloc(sizeof(Memory));
	mem->size = 0;
	mem->top = NULL;
	return mem;
}


int is_empty_mem(Memory *mem){
	return mem->top==NULL;
}


void destroy_node(node *node){
	free(node);
}

void destroy_list(list *list){
	node *curr = list->top;
	node *temp;
	while(curr!=NULL){
		temp = curr;
		list->top = curr->next;
		destroy_node(temp);
	}
}

void pop_and_destroy_list(Memory *mem, list *list){
	/* base case */
	    if (mem->top== NULL || list == NULL)


	    /* If node to be deleted is head node */
	    if (mem->top == list)
	    	mem->top = list->next;

	    /* Change next only if node to be deleted is NOT the last node */
	    if (list->next != NULL)
	        list->next->prev = list->prev;

	    /* Change prev only if node to be deleted is NOT the first node */
	    if (list->prev != NULL)
	        list->prev->next = list->next;

	    /* Finally, free the memory occupied by del*/
	   destroy_list(list);
}





/*
void pop_and_destroy_list(list *list){

	list->next->prev = list->prev;
	list->prev->next = list->next;
	destroy_list(list);
}*/

void push_mem(Memory *mem, list *newList)
{
	/*list *curr;*/
	/*push the new list to mem*/
    newList->next = mem->top;
    newList->prev = NULL;
    if(mem->top!=NULL){
    	mem->top->prev=newList;
    }
    mem->top = newList;
    mem->currentPointer = newList;
    mem->size++;
/*

	if(mem->currentPointer!=NULL){
	    curr = mem->currentPointer->prev;
	    while(curr!=mem->top){
	    	printf("prev");
	    	pop_and_destroy_list(mem, curr);
	    	printf("after dest");
	    	curr = curr->prev;
	    }
	}*/




}

void push_to_list(list *list, node *node){
	if(list->top!=NULL){
		node->next = list->top;
	}
	list->top = node;
	list->size++;
}


void update_list(list *list, int x, int y, int z, int oldVal){
	node *newNode;
	newNode = create_node(x, y, z, oldVal);
	push_to_list(list,newNode);
}



node* create_node(int col, int row, int newVal, int oldVal){
	node *elem=(node*)malloc(sizeof(node));
	elem->col = col;
	elem->row = row;
	elem->newVal=newVal;
	elem->oldVal = oldVal;
	elem->next=NULL;
	return elem;
}


list* create_list(){
	list *newList=(list*)malloc(sizeof(list));
	/*create new list */
	newList->top=NULL;
	newList->size = 0;
	newList->next=NULL;
	newList->prev=NULL;
	newList->undoed=0;
	newList->redoed=0;
	return newList;
}
/*
void destroy_mem(Memory *mem) {
	int memSize = mem->size;
	int i,j;
	node *tempNode = (node*)malloc(sizeof(node));
	list *tempList = (list*)malloc(sizeof(list));

	for (i =0; i < memSize; i++) {
		tempList = mem->top->next;


		for (j = 0; j < mem->top->size; j++) {
			tempNode = mem->top->top->next;
			free(mem->top->top);
			mem->top->top = tempNode;
		}

		free(mem->top);
		mem->top = tempList;
		mem->currentPointer = mem->top;
	}


free (tempNode);
free (tempList);


}

*/

