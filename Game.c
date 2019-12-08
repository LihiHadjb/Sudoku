/*
 * Game.c
 *



 * Game.c
 *
 */
#include "Game.h"

/*return 1 iff the board contains errors*/
int check_erroneous(Board *board){
	if(is_erroneous(board)){
		erroneous_board_mssg();
		return 0;
	}
	return 1;
}


/*return 1 iff the command is one that updated the board*/
int is_updating(Command command){
	int t= get_type_code(command);
	return t==SET || t==AUTOFILL || t==UNDO || t==REDO || t==GENERATE || t==GUESS || t==RESET || t==EDIT || t==SOLVE;
}



int manage_game(Game *game){
	Command comm;
	int succ=1, m=-1, n=-1, num_empty=-1;
	SP_BUFF_SET();
	welcome_mssg();
	while(succ!=-1){
		if(game->board_initialized){
			m = game->curr_board->m;
			n = game->curr_board->n;
			num_empty = get_num_empty_cells(game->curr_board);
		}
		comm = get_command_from_user(game->mode, m, n, num_empty);
		succ = exec_command(game, comm);
		if(succ==1){
			/*add command to undo list*/
		}
		if (is_updating(comm) && succ==1){
			print_board_comm(game);
			if(game->mode == MODE_SOLVE && is_finished(game->curr_board)){
				if(!is_erroneous(game->curr_board)){
					solved_mssg();
					game->mode = MODE_INIT;

				}
				else{
					erroneous_full_mssg();
				}
			}
		}
		free(comm);
	}
	return 1;
}



Game* create_Game(){
	Game *game = (Game*)malloc(sizeof(Game));
	if(game == NULL) {memory_error("malloc");}
	game->mode = MODE_INIT;
	game->curr_board = (Board*)malloc(sizeof(Board));
	if(game->curr_board == NULL) {memory_error("malloc");}
	game->mark_errors = 1;
	game->board_initialized = 0;
	game->mem = init_mem();
	return game;
}


/*free all memory resources related to the game*/
void destroy_Game(Game *game){
	destroy_Board(game->curr_board);
	/*destroy_mem(game->mem);*/
	free(game);
}




int exec_command(Game *game, Command comm){
	int type = get_type_code(comm);
	switch(type){
	case(SOLVE):
					return solve_comm(game, parse_path(comm));
	case(EDIT):
					return edit_comm(game, parse_path(comm));
	case (MARK_ERRORS):
					return mark_errors(game, parse_x(comm));
	case(PRINT_BOARD):
					return print_board_comm(game);
	case(SET):
					return set(game, parse_x(comm), parse_y(comm), parse_z(comm));
	case(VALIDATE):
					return validate(game);
	case(GUESS):
					return guess(game, parse_guess(comm));
	case(GENERATE):
					return generate(game, parse_x(comm), parse_y(comm));
	case(UNDO):
					return undo(game);
	case(REDO):
					return redo(game);
	case(SAVE):
					return save(game, parse_path(comm));
	case(HINT):
					return hint(game, parse_x(comm), parse_y(comm));
	case(GUESS_HINT):
					return guess_hint(game, parse_x(comm), parse_y(comm));
	case(NUM_SOLUTIONS):
					return num_solutions(game);
	case(AUTOFILL):
					return autofill(game);
	case(RESET):
					return reset(game);
	case(EXIT):
					return exit_game(game);
	}
	return 0;
}


/*load a board from a file in path*/
/*return 1 iff the operation succeeded*/
int load_to_game(Game *game, char *path){
	Board *temp;
	temp = load_board(path, game->mode);

	/*if the load failed, keep the old board*/
	if(temp == NULL){
		return 0;
	}

	/*load succeeded and this is the first board ever(i.e. if this is the first board in the game)*/
	if(!game->board_initialized){
		game->curr_board = temp;
		game->board_initialized = 1;
	}

	/*load succeeded and there already is board */
	else{

		destroy_Board(game->curr_board);
		game->curr_board = temp;
		game->board_initialized = 1;
	}

	return 1;
}


/*load a board from path x and switch to solve mode*/
/*return 1 iff the operation succeeded*/
int solve_comm(Game *game, char* x){
	game->mode = MODE_SOLVE;
	return load_to_game(game, x);
}



/*load a board from path x and switch to edit mode*/
/*return 1 iff the operation succeeded*/
int edit_comm(Game *game, char* path){
	game->mode = MODE_EDIT;

	/*no argument supplied*/
	if(path == NULL){
		if(game->board_initialized){
			destroy_Board(game->curr_board);
		}
		game->curr_board = create_Board(DEFAULT_M,DEFAULT_N);
		game->board_initialized = 1;
		return 1;
	}

	/*argument supplied*/
	else{
		return load_to_game(game, path);
	}
}


/*print the changes done by the last autofill command*/
void print_autofill_changes(Game *game){
	list *step = game->mem->top;
	node *curr = step->top;
	autofill_changes_mssg();
	while(curr!=NULL){
		board_change_mssg(curr->col, curr->row, curr->newVal);
		curr = curr->next;
	}
}



int autofill(Game *game){
	if(!check_erroneous(game->curr_board)){
		return 0;
	}
	fill_obvious_cells(game->curr_board, game->mem);
	print_autofill_changes(game);
	return 1;
}


int exit_game(Game *game){
	destroy_Game(game);
	printf("Exiting...\n");
	return -1;
}







/*Generate a puzzle by filling x empty cells and cleaning all but y cells from the resulting board.
 return 1 iff the operation succeeded*/
int generate(Game *game, int x, int y){
	Board **res_board=(Board**)malloc(sizeof(Board*));
	if(is_erroneous(game->curr_board) || !generate_board(game->curr_board,res_board, x, y)){
		generate_error_mssg();
		return 0;
	}
	add_generate_changes_to_mem(game->mem, *res_board, game->curr_board);
	destroy_Board(game->curr_board);
	game->curr_board = *res_board;
	free(res_board);
	return 1;
}









/*return 0 iff cell x, y on board is fixed, and notify the user if it is*/
int check_fixed(Board *board, int x, int y){
	if(is_fixed_cell(board, x, y)){
		fixed_cell_mssg(x, y);
		return 0;
	}
	return 1;
}
/*return 0 iff cell x, y on board is  not empty, and notify the user if it is*/
int check_empty_cell(Board *board, int x, int y){
	if(!is_empty_cell(board, x, y)){
			not_empty_cell_mssg(x, y);
			return 0;
		}
	return 1;
}

/*check if the board is valid for perfoming a hint command*/
int check_board_for_hint(Game *game, int x, int y){
	return (check_erroneous(game->curr_board) && check_fixed(game->curr_board, x, y) && check_empty_cell(game->curr_board, x, y));
}


int hint(Game *game, int x, int y){
	int hint;
	Board *board = game->curr_board;
	Options_Board *opts_board;
	double **sol = (double**)malloc(sizeof(double*));
	if(sol == NULL) {memory_error("malloc");}

	if(!check_board_for_hint(game, x, y)){
		return 0;
	}


	opts_board=create_Options_Board(board->m, board->n);
	if(!call_GRB(board, sol, ILP, opts_board)){
		no_sol_mssg();
		return 0;
	}

	hint = get_sol_for_cell(opts_board, sol, x, y);
	give_hint_mssg(x, y, hint);
	free(sol);
	destroy_Options_Board(opts_board);
	return 1;

}

/*solve the board with LP and notify the user of all the possible legal assignment to cell x,y that
 * got a score>0
 */
int guess_hint(Game *game, int x, int y){
	int num_elems;
	int **values;
	double **scores;

	Options_Board *opts_board;
	Board *board = game->curr_board;
	double **sol = (double**)malloc(sizeof(double*));
	if(sol == NULL) {memory_error("malloc");}

	if(!check_board_for_hint(game, x, y)){
		return 0;
	}

	opts_board = create_Options_Board(board->m, board->n);
	if(!call_GRB(board, sol, LP, opts_board)){
		no_sol_mssg();
		/*free(sol);*/
		return 0;
	}

	values = (int**)malloc(sizeof(int*));
	if(values == NULL) {memory_error("malloc");}
	scores = (double**)malloc(sizeof(double*));
	if(scores == NULL) {memory_error("malloc");}
	num_elems = get_scores_for_cell(opts_board,sol, scores, values, x, y, 0);
	give_guess_hint_mssg(x, y, scores, values, num_elems);

	free(*scores);
	free(*values);
	free(scores);
	free(values);
	destroy_Options_Board(opts_board);
	return 1;
}



int validate(Game *game){
	Board *board;
	/*Board *copy;*/
	Options_Board *opts_board;
	double **sol = (double**)malloc(sizeof(double*));
	if(sol == NULL) {memory_error("malloc");}
	board = game->curr_board;
	/*copy = get_board_copy(board);*/
	if(!check_erroneous(game->curr_board)){
		return 0;
	}


	opts_board=create_Options_Board(board->m, board->n);
	if(call_GRB(board, sol, ILP, opts_board)){
		validation_succ_mssg();
		/*fill_solution(sol, copy, opts_board);
		print_board(copy,1);*/
		destroy_Options_Board(opts_board);
		return 1;

	}
	else{
		validation_failed_mssg();
		destroy_Options_Board(opts_board);
		return 0;
	}
}

/*remove illegal values created along the way during the guess operation, according to scores, for cell col, row*/
int remove_illegal_values(Board *board, double **scores, int **values, int num_elems, int col, int row){
	int *new_values;
	double *new_scores;
	int i, legal_cnt=0, new_size, curr=0;

	/*count the number of elems to keep in the arrays*/
	for(i=0; i<num_elems; i++){
		if(is_legal_set(board, col, row, (*values)[i]) && (*scores)[i]>0){
			legal_cnt++;
		}
	}

	/*remove illegal values from the arrays*/
	new_size = legal_cnt;
	new_values = (int*)malloc(sizeof(int)*new_size);
	if(new_values == NULL) {memory_error("malloc");}
	new_scores = (double*)malloc(sizeof(double)*new_size);
	if(new_scores == NULL) {memory_error("malloc");}
	for(i=0; i<num_elems; i++){
		if(is_legal_set(board, col, row, (*values)[i]) && (*scores)[i]>0){
			new_values[curr] = (*values)[i];
			new_scores[curr] = (*scores)[i];
			curr++;
		}
	}

	*values = new_values;
	*scores = new_scores;
	return new_size;
}





int guess (Game *game, float threshold){
	Options_Board *opts_board;
	int **values;
	double **scores;
	int num_elems, i, j, ind;
	Board *board = game->curr_board;
	int board_size = board->m * board->n;
	double **sol = (double**)malloc(sizeof(double*));
	if(sol == NULL) {memory_error("malloc");}

	if(!check_erroneous(game->curr_board)){
		return 0;
	}

	opts_board = create_Options_Board(board->m, board->n);
	if(!call_GRB(board, sol, LP, opts_board)){
		no_sol_mssg();
		destroy_Options_Board(opts_board);
		return 0;
	}

	values = (int**)malloc(sizeof(int*));
	if(values == NULL) {memory_error("malloc");}
	scores = (double**)malloc(sizeof(double*));
	if(scores == NULL) {memory_error("malloc");}
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){

			if(is_empty_cell(board, j, i)){
				num_elems = get_scores_for_cell(opts_board,sol, scores, values, j, i, threshold);
				/*print_arrays(scores, values, num_elems, 0);*/
				num_elems = remove_illegal_values(board, scores, values, num_elems, j, i);
				if(num_elems == 0){
					continue;
				}
				ind = get_rand_index_with_probs(*scores, num_elems);
				set_cell(board, j, i, (*values)[ind]);

			}
		}
	}
	destroy_Options_Board(opts_board);
	return 1;
}








int num_solutions(Game *game){
	int result;
	if(is_erroneous(game->curr_board)){
		erroneous_board_mssg();
		return 0;
	}
	result = count_solutions(game->curr_board);
	num_sols_mssg(result);
	return 1;
}




/*auxiliary function for undo*/
void undo_node(Game *game, node *node){
	int col, row, oldVal;
	col = node->col;
	row= node->row;
	oldVal = node->oldVal;
	node->oldVal = get_cell(game->curr_board, col, row);
	set_cell(game->curr_board, col, row, oldVal);
	board_change_mssg(col, row, oldVal);
}

/*undo a full step done by the user. auxiliary function for undo*/
void undo_list(Game *game, list *step){
	node *curr;
	curr=step->top;
	while(curr!=NULL){
		undo_node(game, curr);
		curr = curr->next;
	}
	step->undoed = 1;
}


int undo(Game *game){
	list *lastStep;
	if(game->mem->currentPointer == NULL){
		can_not_undo_mssg();
		return 0;
	}

	lastStep = game->mem->currentPointer;
	undo_list(game, lastStep);
	game->mem->currentPointer = lastStep->next;
	return 1;
}

/*auxiliary function for redo*/
void redo_node(Game *game, node *node){
	int col, row, newVal;
	col = node->col;
	row= node->row;
	newVal = node->newVal;
	set_cell(game->curr_board, col, row, newVal);
	board_change_mssg(col, row, newVal);
}
/*redo a full step done by the user. auxiliary function for undo*/
void redo_list(Game *game, list *step){
	node *curr;
	curr=step->top;
	while(curr!=NULL){
		redo_node(game, curr);
		curr=curr->next;
	}
	step->redoed = 1;
}

/*undo the last step done by the user*/
int redo(Game *game){
	list *lastStep;

	if(game->mem->currentPointer == NULL || game->mem->currentPointer==game->mem->top){
			can_not_redo_mssg();
			return 0;
	}
	lastStep = game->mem->currentPointer;
	game->mem->currentPointer = lastStep->prev;
	lastStep = game->mem->currentPointer;
	redo_list(game, lastStep);
	return 1;
}


int reset(Game *game){
	return game->is_solved;
}


int save(Game *game, char* x){
	Board *copy;
	Board *board = game->curr_board;
	Options_Board *opts_board;
	double **sol = (double**)malloc(sizeof(double*));
	if(sol == NULL) {memory_error("malloc");}

	if(game->mode == MODE_EDIT && is_erroneous(board)){
		can_not_save_erroneous_mssg();
		return 0;
	}

	opts_board = create_Options_Board(board->m, board->n);
	if(game->mode == MODE_EDIT && !call_GRB(board, sol, ILP, opts_board)){
		can_not_save_unsolvable_mssg();
		return 0;
	}

	if(game->mode == MODE_EDIT){
		copy = get_board_copy(board);
		fix_all_non_empty(copy);
		save_board(copy, x);
		destroy_Board(copy);
	}

	else{/*mode is SOLVE*/
		save_board(board, x);
	}
	saved_successfully_mssg(x);
	return 1;
}


int mark_errors(Game *game, int x){
	game->mark_errors = x;
	return 1;
}


int get_mark_errors(Game* game){
	if(game->mode == MODE_EDIT || game->mark_errors == 1){
		return 1;
	}
	return 0;
}

int print_board_comm(Game *game){
	if(!game->board_initialized){
		no_board_mssg();
		return 0;
	}
	print_board(game->curr_board, get_mark_errors(game));
	return 1;
}





int set(Game *game, int x, int y, int z){
	int oldVal;
	list *step = create_list();
	if (/*game->mode == MODE_SOLVE && */is_fixed_cell(game->curr_board,x,y)) {
		fixed_cell_mssg(x, y);
		return 0;
	}
	oldVal = get_cell(game->curr_board, x, y);
	set_cell(game->curr_board,x,y,z);

	/*update steps memory*/
	update_mem(oldVal, x, y, z, step);
	set_cell(game->curr_board,x,y,z);
	push_mem(game->mem, step);
	return 1;
}
