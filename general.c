/*
 * general.c
 *
 *  Created on: 26 באוג׳ 2019
 *      Author: ליהי
 */
#include "general.h"

void memory_error(const char* func_name){
	printf("Error: %s has failed\n", func_name);
	exit(0);
}


void init_str_mem(char *str, int size){
	int i;
	for(i=0; i<size; i++){
		str[i] = '\0';
	}
}
