/*
 * Parser.c
 *
 *  Created on: 22  2019
 *      Author:
 */

#include "Parser.h"



/*Returns a copy of the command src*/
char* get_cpy(const Command src){
	char* cpy = (char*)malloc(sizeof(char)*strlen(src));
	if(cpy == NULL) {memory_error("malloc");}
	strcpy(cpy, src);
	return cpy;
}




 int get_type_code(const Command comm_str){
	int i;
	char *comm_str_cpy, *comm_type;
	char* types [] = {"solve","edit","mark_errors","print_board","set","validate","guess","generate","undo","redo","save","hint","guess_hint","num_solutions","autofill","reset","exit"};
	comm_str_cpy= get_cpy(comm_str);
	comm_type = strtok(comm_str_cpy, DELIM);
	for(i=0; i<NUM_TYPES; i++){

		if(strcmp(types[i], comm_type)==0){
			/*free(comm_str_cpy);*/
			return i+1;
		}
	}
	free(comm_str_cpy);
	return 0;
}









/*Returns 1 iff the first argument given in comm_str is an
 *integer, and notifies the user if not*/
int check_x_is_integer(const Command comm_str){
	if(parse_x(comm_str) == -1){
			not_an_int_mssg("First", 0);
			return 0;
	}
	return 1;
}


/*Returns 1 iff the second argument given in comm_str is an
 *integer, and notifies the user if not*/
int check_y_is_integer(const Command comm_str){
		if(parse_y(comm_str) == -1){
			not_an_int_mssg("Second", 0);
			return 0;
		}
	return 1;
}

/*Returns 1 iff the third argument given in comm_str is an
 *integer, and notifies the user if not*/
int check_z_is_integer(const Command comm_str){
	if(parse_z(comm_str) == -1){
		not_an_int_mssg("Third", 1);
		return 0;
	}
	return 1;
}


/*Returns 1 iff the first argument given in comm_str is a
 *float, and notifies the user if not*/
int check_x_for_guess(const Command comm_str){
	if(parse_guess(comm_str) == -1){
		invalid_threshold_mssg();
		return 0;
	}
	return 1;
}



/*Return 1 iff 1<=x<=m*n. This is used for checking whether x is a valid coordiante in a board
 * or a valid value for a cell in a board*/
int in_range(int x, int m, int n){
	return (x>=1 && x<=m*n);
}

/*Return 1 iff z is a legal value for a cell in a board with dims m,n , and notifies the user if not*/
int is_value_for_cell(int z, int m, int n){
	if(!(in_range(z, m, n) || z == 0)){
		argument_range_mssg(0,m*n,"third","The value to set is illegal");
		return 0;
	}
	return 1;
}

/*Return 1 iff the argument of the command given in comm is legal for mark_errors
 *and notifies the user if not*/
int check_mark_errors_args(Command comm){
	int x;
	if(!check_x_is_integer(comm)){
		return 0;
	}
	x=parse_x(comm);
	if(!(x == 0 || x == 1)){
		invalid_mark_mssg();
		return 0;
	}
	return 1;
}

/*Return 1 iff the arguments of the command given in comm are legal for set and a board
 * with dims m, n, and notifies the user if not*/
int check_set_args(Command comm, int m, int n){
	int x, y, z;
	/*check x*/
	if(!check_x_is_integer(comm) ){
		return 0;
	}
	x = parse_x(comm);
	if(!(in_range(x, m, n))){
		argument_range_mssg(1,m*n,"First","There is no such cell");
		return 0;
	}

	/*check y*/
	if(!check_y_is_integer(comm)){
			return 0;
	}
	y=parse_y(comm);
	if(!(in_range(y, m, n))){
		argument_range_mssg(1,m*n,"Second","There is no such cell");
		return 0;
	}

	/*check z*/
	if(!check_z_is_integer(comm)){
		return 0;
	}
	z=parse_z(comm);
	if(!is_value_for_cell(z, m, n)){
		return 0;
	}

	return 1;
}






/*Return 1 iff the arguments of the command given in comm are legal for hint and a board
 * with dims m, n, and notifies the user if not*/
int check_hint_args(Command comm, int m, int n){
	int x, y;

	/*check x*/
	if(!check_x_is_integer(comm)){
		return 0;
	}
	x = parse_x(comm);
	if(!(in_range(x, m, n))){
		argument_range_mssg(1,m*n,"First","There is no such cell");
		return 0;
	}

	/*check y*/
	if(!check_y_is_integer(comm)){
			return 0;
	}
	y=parse_y(comm);
	if(!(in_range(y, m, n))){
		argument_range_mssg(1,m*n,"Second","There is no such cell");
		return 0;
	}

	return 1;
}

/*Returns 1 iff threshold has a legal value (i.e. 0<=threshold<=1)*/
int check_threshold(float threshold){
	if (!(threshold>=0 && threshold<=1)){
		invalid_threshold_mssg();
		return 0;
	}
	return 1;
}



/*Return 1 iff the arguments of the command given in comm are legal for guess and a board
 * with dims m, n, and notifies the user if not*/
int check_guess_args(Command comm){
	float x;
	if(!check_x_for_guess(comm)){
		return 0;
	}
	x=parse_guess(comm);
	return check_threshold(x);
}

/*Return 1 iff the argument x (that specifices the number of empty cells to fill) is legal for
 * the board, according to num_empty, and notifies the user if not*/
int check_generate_x(int x, int num_empty){
	if(x > num_empty){
			not_enough_empty_cells_mssg(num_empty);
			return 0;
	}
	return 1;
}


/*Return 1 iff the argument y (that specifices the number of cells to keep after
 * generating a puzzle) is legal for the board, according to its
 * dims, and notifies the user if not*/
int check_generate_y(int y, int m, int n){
	int board_size = m*n;
	if(y > board_size*board_size){
			not_enough_cells_mssg(board_size*board_size);
			return 0;
	}
	return 1;
}


/*Return 1 iff the arguments of the command given in comm are legal for generate and a board
 * with dims m, n and num_empty empty cells, and notifies the user if not*/
int check_generate_args(Command comm, int m, int n, int num_empty){
	int x, y;

	/*check x*/
	if(!check_x_is_integer(comm)){
		return 0;
	}
	x = parse_x(comm);
	if(!check_generate_x(x, num_empty) ){
		return 0;
	}

	/*check_y*/
	if(!check_y_is_integer(comm)){
		return 0;
	}
	y = parse_y(comm);
	if(!check_generate_y(y,m, n) ){
		return 0;
	}

	return 1;
}



/*returns the expected number of arguments for the command comm_str according to the desired type of
 * arguments: mandaoty or options, that is given in is_mandatory*/
int get_num_args(const Command comm_str, int is_mandatory){
	int result=0;
	int comm_type = get_type_code(comm_str);
	switch(comm_type){
	case(SOLVE):
		result=(is_mandatory==1? 1:0);
		break;
	case(EDIT):
		result=(is_mandatory==1? 0:1);
		break;
	case (MARK_ERRORS):
		result=(is_mandatory==1?1:0);
					break;
	case(PRINT_BOARD):
		result=(is_mandatory==1? 0:0);
					break;
	case(SET):
		result=(is_mandatory==1? 3:0);
					break;
	case(VALIDATE):
		result=(is_mandatory==1? 0:0);
					break;
	case(GUESS):
		result=(is_mandatory==1? 1:0);
					break;
	case(GENERATE):
		result=(is_mandatory==1? 2:0);
					break;
	case(UNDO):
		result=(is_mandatory==1? 0:0);
					break;
	case(REDO):
		result=(is_mandatory==1? 0:0);
					break;
	case(SAVE):
		result=(is_mandatory==1? 1:0);
					break;
	case(HINT):
		result=(is_mandatory==1? 2:0);
					break;
	case(GUESS_HINT):
		result=(is_mandatory==1? 2:0);
					break;
	case(NUM_SOLUTIONS):
		result=(is_mandatory==1? 0:0);
					break;
	case(AUTOFILL):
		result=(is_mandatory==1? 0:0);
					break;
	case(RESET):
		result=(is_mandatory==1? 0:0);
					break;
	case(EXIT):
		result=(is_mandatory==1? 0:0);
					break;
	}
	return result;
}




/*returns a string with the name of the command with type code type*/
char* type_code_to_name(int type){

	switch(type){
	case(SOLVE):
					return "solve";
	case(EDIT):
					return "edit";
	case (MARK_ERRORS):
					return "mark_errors";
	case(PRINT_BOARD):
					return "print_board";
	case(SET):
					return "set";
	case(VALIDATE):
					return "validate";
	case(GUESS):
					return "guess";
	case(GENERATE):
					return "generate";
	case(UNDO):
					return "undo";
	case(REDO):
					return "redo";
	case(SAVE):
					return "save";
	case(HINT):
					return "hint";
	case(GUESS_HINT):
					return "guess_hint";
	case(NUM_SOLUTIONS):
					return "num_solutions";
	case(AUTOFILL):
					return "autofill";
	case(RESET):
					return "reset";
	case(EXIT):
					return "exit";
	}
	return NULL;
}






/*Replaces any space character with a whitespace*/
void remove_tabs(char *str){
	int size = strlen(str);
	int i;
	for(i=0; i<size; i++){
		if(isspace(str[i])){
			str[i]=' ';
		}
	}
}

/*Returns 1 iff the command given in comm_str is of legal length, and notifies the user if not*/
int check_len(const Command comm_str){
	if(strlen(comm_str) > MAX_COMMAND_SIZE){
		invalid_len_mssg(MAX_COMMAND_SIZE);
		return 0;
	}
	return 1;
}

/*Returns 1 iff the name of the command given in comm_str has
 * a legal name (i.e. such command exists), and notifies the user if not*/
int check_name(const Command comm_str){
	if(get_type_code(comm_str)==0){
		invalid_name_mssg();
		return 0;
	}
	return 1;
}

/*Returns 1 iff the number of arguments given in comm_str matches the expected number
 * of arguments, and notifies the user if not*/

int check_num_args(const Command comm_str){
	int mands, opts;
	int cnt = 0;
	char *comm_str_cpy= get_cpy(comm_str);
	int type = get_type_code(comm_str);
    char *ptr, *name;
    name = type_code_to_name(type);

    /*count the number of arguments by tokenizing the string*/
    ptr = strtok(comm_str_cpy, DELIM);
    ptr = strtok(NULL, DELIM);/*get the command name out*/
    while(ptr != NULL)
    {
    	cnt++;
    	ptr = strtok(NULL, DELIM);
    }

    /*get the desired number of args for this command*/
	mands = get_num_args(comm_str,1);
	opts = get_num_args(comm_str,0);

	/*too many arguments*/
	if(cnt > mands +opts){
		too_many_args_mssg(name, mands, opts);
	}

	/*missing arguments*/
	else if(cnt < mands){
		missing_args_mssg(name, mands, opts);
	}

	else{
		return 1;
	}
	free(comm_str_cpy);
	return 0;
}

/*Return 1 iff the command specified in comm_str is available when in the game mode specified
 * in mode*/
int get_modes(const Command comm_str, int mode){
	int type = get_type_code(comm_str);
	switch(type){
		case(SOLVE):
			return 1;
		case(EDIT):
			return 1;
		case (MARK_ERRORS):
			return mode == MODE_SOLVE;
		case(PRINT_BOARD):
			return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(SET):
			return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(VALIDATE):
			return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(GUESS):
			return mode == MODE_SOLVE;
		case(GENERATE):
			return mode == MODE_EDIT;
		case(UNDO):
			return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(REDO):
			return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(SAVE):
			return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(HINT):
			return mode == MODE_SOLVE;
		case(GUESS_HINT):
			return mode == MODE_SOLVE;
		case(NUM_SOLUTIONS):
				return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(AUTOFILL):
				return mode == MODE_SOLVE;
		case(RESET):
				return mode == MODE_EDIT || mode == MODE_SOLVE;
		case(EXIT):
				return 1;
	}
	return 0;
}

/*Returns 1 iff the command in comm_str is available in the game mode in mode,
 *and notifies the user if not*/
int check_mode(const Command comm_str, int mode){
	if(!get_modes(comm_str, mode)){
		invalid_mode_mssg(get_modes(comm_str, MODE_INIT),get_modes(comm_str, MODE_EDIT),get_modes(comm_str, MODE_SOLVE));
		return 0;
	}
	return 1;
}


/*Returns 1 iff comm_str is a blank line*/
int is_blank_line(const Command comm_str){
	int i;
	for (i=0; i<(int)strlen(comm_str); i++){
		if(!isspace(comm_str[i])){
			return 0;
		}
	}
	return 1;
}

/*Returns 1 iff the argument of the the command in comm are in correct type and range
 * for this command and a board with dims m,n and num_empty empty cells*/
int check_args_ranges(const Command comm, int m, int n, int num_empty){
	int comm_type = get_type_code(comm);
	switch(comm_type){
			case (MARK_ERRORS):
				return check_mark_errors_args(comm);
			case(SET):
				return check_set_args(comm ,m, n);
			case(GENERATE):
				return check_generate_args(comm, m, n, num_empty);
			case(GUESS):
				return check_guess_args(comm);
			case(HINT):
				return check_hint_args(comm, m, n);
			case(GUESS_HINT):
				return check_hint_args(comm, m, n);
			default:
				return 1;
		}
		return 0;
}


/*Returns 1 iff all the requirments from the command in comm_str are fullfiled, according to
 * the order specified in the project's insturctions, and with respect to the game mode in mode and
 * a board with dims m,n and num_empty empty_cells.
 * This functions calls all the other relevant functions to produce messages in the right order*/
int check_command(const Command comm_str, int mode, int m, int n, int num_empty){
	return check_len(comm_str) && check_name(comm_str) && check_mode(comm_str, mode) && check_num_args(comm_str) && check_args_ranges(comm_str, m, n, num_empty);

}


Command get_command_from_user(int curr_mode, int m, int n, int num_empty){
	char *comm_str;
	int valid = 0;

	SP_BUFF_SET();
	while(!valid){

		comm_str = malloc((MAX_COMMAND_SIZE+2)*sizeof(char));/*this memory is freed later in 'manage game'*/
		if(comm_str == NULL) {memory_error("malloc");}
		enter_command_prompt();
		/*get the command entered by the user*/
		if(fgets(comm_str, MAX_COMMAND_SIZE+2, stdin) == NULL){
			free(comm_str);
			return NULL;
		}

		/*clean the input buffer in case the command was too long*/
		if (!strchr(comm_str, '\n')) {
			int ch;
			while (((ch = getchar()) != EOF) && (ch != '\n'));
		}

		/*handle blank lines separately*/
		if(is_blank_line(comm_str)){
			continue;
		}

		else{
			remove_tabs(comm_str);
			/*remove_spaces_from_end(comm_str);*/
		}
		valid = check_command(comm_str, curr_mode, m,n, num_empty);
		/*remove: free failed command!!!!*/
	}
	return comm_str;
}



int parse_x(const Command comm_str){
	char *ptr, *end_ptr;
	int result;
	char *comm_str_cpy= get_cpy(comm_str);
	SP_BUFF_SET();
	/*remove_tabs_from_beg(comm_str_cpy);*/
	ptr = strtok(comm_str_cpy, DELIM);/*ptr=name*/
	ptr = strtok(NULL, DELIM);/*ptr=x*/
	result = strtol(ptr, &end_ptr, 10);
	if (!(result >= 0 && ptr != end_ptr && *end_ptr == '\0')) {
		return -1;
	}
	free(comm_str_cpy);
	return result;
}



int parse_y(const Command comm_str){
	char *ptr, *end_ptr;
	int result;
	char *comm_str_cpy= get_cpy(comm_str);
	SP_BUFF_SET();
	/*remove_tabs_from_beg(comm_str_cpy);*/
	ptr = strtok(comm_str_cpy, DELIM);/*ptr=name*/
	ptr=strtok(NULL, DELIM);/*ptr=x*/
	ptr=strtok(NULL, DELIM);/*ptr=y*/
	result = strtol(ptr, &end_ptr, 10);
	if (!(result >= 0 && ptr != end_ptr && *end_ptr == '\0')) {
		return -1;
	}
	free(comm_str_cpy);
	return result;
}





int parse_z(const Command comm_str){
	char *ptr, *end_ptr;
	int result;
	char *comm_str_cpy= get_cpy(comm_str);
	/*remove_tabs_from_beg(comm_str_cpy);*/
	ptr = strtok(comm_str_cpy, DELIM);/*ptr=name*/
	ptr=strtok(NULL, DELIM);/*ptr=x*/
	ptr=strtok(NULL, DELIM);/*ptr=y*/
	ptr=strtok(NULL, DELIM);/*ptr=z*/
	result = strtol(ptr, &end_ptr, 10);
	if (!(result >= 0 && ptr != end_ptr && *end_ptr == '\0')) {
		return -1;
	}
	free(comm_str_cpy);
	return result;
}

 char* parse_path(const Command comm_str){
	char *ptr;
	char *comm_str_cpy= get_cpy(comm_str);
	ptr = strtok(comm_str_cpy, DELIM);/*ptr=name*/
	ptr=strtok(NULL, DELIM);/*ptr=x*/
	/*free(comm_str_cpy);*/
	return ptr;
}



 float parse_guess(const Command comm_str){
	char *ptr, *end_ptr;
	int result;
	char *comm_str_cpy= get_cpy(comm_str);
	SP_BUFF_SET();
	/*remove_tabs_from_beg(comm_str_cpy);*/
	ptr = strtok(comm_str_cpy, DELIM);/*ptr=name*/
	ptr=strtok(NULL, DELIM);/*ptr=x*/
	result = strtod(ptr, &end_ptr);
	if (!(result >= 0 && ptr != end_ptr && *end_ptr == '\0')) {
		return -1;
	}
	free(comm_str_cpy);
	return result;
}




