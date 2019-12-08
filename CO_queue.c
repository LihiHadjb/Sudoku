

#include "CO_queue.h"


/*Create and return an Option object, which represents a variable in the Gurobi model. val is the
 * value of the legal assignment to the cell the queue belongs to, and idx is its identifier among
 * all the variables*/
Option* create_Option(int idx, int val, Option* next){
	Option* elem;
	elem = (Option*)malloc(sizeof(Option));
	if (elem == NULL) {memory_error("malloc");}

	elem->next = next;
	elem->value = val;
	elem->idx = idx;
	return elem;
}

/*Free all memory resources related to opt*/
void destroy_Option(Option *opt){
	/*free(opt->next);*/
	free(opt);
}


/*Create and return a clone of option*/
Option* copy_Option(Option *option){
	Option *cpy = (Option*)malloc(sizeof(Option));
	if (cpy == NULL) {memory_error("malloc");}
	cpy->idx = option->idx;
	cpy->value =option->value;
	cpy->next = NULL;
	return cpy;
}

/*insert a new element to the end of the queue*/
void insert(CO_queue *q, Option *opt){
	if(q->size == 0){

		q->first = opt;
		q->last = opt;
	}
	else{

		q->last->next = opt;
		q->last = opt;
	}

	q->size++;
}


/*pop the first element out of the queue and return it*/
/*assumes q is not empty!!!*/
Option* dequeue(CO_queue *q){
	Option *first;
	first = q->first;
	q->first =q->first->next;
	if(q->first == NULL){
		q->last = NULL;
	}
	q->size--;
	return first;
}

/*return 1 iff the queue contains no elements*/
int is_empty_Q(CO_queue *q){
	return q->size == 0;
}

/*Free all memory resources related to q*/
void destroy_CO_queue(CO_queue *q){
	while(!is_empty_Q(q)){
		Option *elem;
		elem = dequeue(q);
		destroy_Option(elem);
	}
	free(q);
}


/*initialze q to start values*/
void init_CO_queue(CO_queue *q){
	q->size = 0;
	q->first = NULL;
	q->last = NULL;
}



/*create a CO_queue object*/
CO_queue* create_CO_queue(){
	CO_queue *q = (CO_queue*)malloc(sizeof(CO_queue));
	if (q == NULL) {memory_error("malloc");}
	init_CO_queue(q);
	return q;
}

/*get the value of the index'th element in the list, without changing the list*/
int CO_get_value_in_index(CO_queue *list,int index){
	int i;
	Option *option = list->first;
	for(i=0; i<index; i++){
		option = option->next;
	}
	return option->value;
}




