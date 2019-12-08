/*
 * Random.c
 *
 *  Created on: 12 בספט׳ 2019
 *      Author: ליהי
 */

#include "Random.h"

/*a helper for  get_rand_index_ints. returns the index of the first cell in arr that is larger than l*/
int get_ceil_idx(int *arr, int r, int l, int h){
    int mid, result = -1;
    while (l < h)
    {
    	mid = (l+h)/2;
    	if(r > arr[mid]){
    		l = mid + 1;
    	}
    	else{
    		h = mid;
    	}

    }
    if(arr[l]>=r){
    	result = l;
    }
    return result;
}

/*return a random number(index) in the range 0 to size, with the probabilities in arr. size is the size of arr*/
int get_rand_index_ints(int *arr, int size){
	int i, r, ceil_idx;


	/* Create prefix array*/
	int *prefix = (int*)malloc(sizeof(int)*size);
	if(prefix == NULL) {memory_error("malloc");}
	prefix[0] = arr[0];
	for(i=1; i<size; i++){
		 prefix[i] = prefix[i - 1] + arr[i];
	}

	r = (rand() % prefix[size - 1]) + 1;
	ceil_idx = get_ceil_idx(prefix, r, 0, size - 1);
	return ceil_idx;
}

/*multiply all the numbers in probs until they are all >1. size is the size of arr*/
int* turn_to_ints(double *probs, int size){
	int i, cnt=0, max_cnt=0;
	double curr;
	int *result = (int*)malloc(sizeof(int) * size);
	if(result == NULL) {memory_error("malloc");}


	for(i=0; i<size; i++){
		curr = probs[i];
		while(curr>0 && curr<1){
			curr = curr*10;
			cnt++;
		}
		if(cnt>max_cnt){
			max_cnt = cnt;
		}
	}
	for(i=0; i<size; i++){
		result[i] = probs[i] * pow(10, max_cnt);
	}
	return result;
}

/*return a random index in the range 0 to size with probalbilities probs for each index*/
int get_rand_index_with_probs(double *probs, int size){
	int idx;
	int *ints;
	ints = turn_to_ints(probs, size);
	idx = get_rand_index_ints(ints, size);
	return idx;
}
