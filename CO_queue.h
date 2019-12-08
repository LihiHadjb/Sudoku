/*
 * CO_queue.h
 *This is a data structure used by the Options_Board structue. It is similliar to a queue.
 *A CO_queue represents a list of all legal values for a specific cell.
 *Insertion to the list is done by adding a new element to the end of the list, and removing an element is done by taking out the first elemet (FIFO)
 *It is also possible to get the value of a specified index in the list by traversing it without changing the list.
 *Each node in the list is an instance of the struct Option, which in turn represents a variable in the ILP/LP system.
 *
 */

#ifndef CO_QUEUE_H_
#define CO_QUEUE_H_


#include "general.h"
#include "Board.h"

/*An Option instance represents a variable in the LP\ILP system.
 * value - the possible legal value for the cell
 * idx - an identifier among all the variables in the model
 */
typedef struct Option{
	int value;
	int idx;
	struct Option *next;
}Option;

/*(Explained above)*/
typedef struct CO_queue{
	int size;
	Option *first;
	Option *last;
}CO_queue;

/*Create an instance of the struct Option*/
Option* create_Option(int idx, int val, Option* next);

/*Free all memory resources related to the Option instance*/
void destroy_Option(Option *opt);

/*Get a "deep clone" of the Option instance*/
Option* copy_Option(Option *option);

/*Add a node to the end of the list*/
void insert(CO_queue *q, Option *opt);

/*Remove the first node in the list*/
Option* dequeue(CO_queue *q);

/*Check if the list contains no elements*/
int is_empty_Q(CO_queue *q);

/*Free all memory resources related to the list*/
void destroy_CO_queue(CO_queue *q);


void init_CO_queue(CO_queue *q);

/*Create an instance of the struct*/
CO_queue* create_CO_queue();

/*Get the value of the element that is currently i'th on the list*/
int CO_get_value_in_index(CO_queue *list, int index);





#endif /* CO_QUEUE_H_ */
