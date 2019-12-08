/*
 * Board.c
 *
 *
 */
#include "Board.h"




void unfix_cell(Board *board, int col, int row) {
	board->fixed_cells[col - 1][row - 1] = 0;
}

void fix_cell(Board *board, int col, int row) {
	board->fixed_cells[col - 1][row - 1] = 1;
}

/*Empty the board. An auxiliary function for create_Board*/
void init_board(Board *board) {
	int i, j;
	int boardSize = board->n * board->m;
	for (i = 1; i <= boardSize; i++) {
		for (j = 1; j <= boardSize; j++) {
			clean_cell(board, i, j);
		}
	}
}

Board* create_Board(int m, int n) {
	int i, boardSize;
	Board *board;


	boardSize = n*m;
	board = (Board*) malloc(sizeof(Board));
	if (board == NULL) {memory_error("malloc");}

	board->numbers = (int**) malloc(boardSize * sizeof(int*));
	board->fixed_cells = (int**) malloc(boardSize * sizeof(int*));

	if (board->fixed_cells == NULL || board->numbers == NULL){memory_error("malloc");}

	for (i = 0; i < boardSize; i++) {
		board->numbers[i] = (int*) malloc(boardSize * sizeof(int));
		board->fixed_cells[i] = (int*) malloc(boardSize * sizeof(int));
		if (board->fixed_cells[i] == NULL || board->numbers[i] == NULL)
			{memory_error("malloc");}

	}
	board->n = n;
	board->m = m;
	init_board(board);
	return board;
}



void destroy_Board(Board *board){
	int i;
	for(i=0; i<board->m*board->n; i++){
		free(board->fixed_cells[i]);
		free(board->numbers[i]);
	}
	free(board->fixed_cells);
	free(board->numbers);
	/*destroy_Cell_options(board->cell_options);*/
	free(board);
}


void update_mem(int oldVal ,int col, int row, int num_to_set, list *step){
	update_list(step, col, row, num_to_set, oldVal);
}


void set_cell(Board *board, int col, int row, int num_to_set) {
	board->numbers[col - 1][row - 1] = num_to_set;
}

int get_cell(Board *board, int col, int row) {
	return board->numbers[col - 1][row - 1];
}

int is_fixed_cell(Board *board, int col, int row) {
	return board->fixed_cells[col - 1][row - 1];
}

int is_empty_cell(Board *board, int col, int row) {
	if (board->numbers[col - 1][row - 1] == 0) {
		return 1;
	}
	return 0;
}

void clean_cell(Board *board, int col, int row) {
	set_cell(board, col, row, 0);
	unfix_cell(board, col, row);
}


/*Check whether the values in cell (col, row) has a neighbour with the same value*/
int is_error(Board *board, int col, int row){
	int val, is_fixed;
	val = get_cell(board, col , row);
	is_fixed = is_fixed_cell(board, col, row);
	clean_cell(board, col, row);
	if(!is_legal_set(board, col, row, val)){
		set_cell(board, col, row, val);
		if(is_fixed){
			fix_cell(board, col, row);
		}
		return 1;
	}
	set_cell(board, col, row, val);
	if(is_fixed){
		fix_cell(board, col, row);
	}
	return 0;
}

/*An auxiliary function for print_board*/
void print_cell(Board *board, int col, int row, int mark_errors) {
	char space = ' ';
	char fixed = '.';
	char error = '*';
	int cell_value = get_cell(board, col, row);
	SP_BUFF_SET();

	printf("%c", space);
	if(is_empty_cell(board, col, row)){
		printf("%c%c", space, space);
	}
	else{
		printf("%2d",cell_value);
	}

	if(is_fixed_cell(board, col, row)){
		printf("%c", fixed);
	}
	else if(mark_errors && !is_empty_cell(board,col,row) && is_error(board, col, row)){
		printf("%c", error);
	}
	else{
		printf("%c", space);
	}
}


void print_board(Board *board, int mark_errors) {
	int i;
	int row = 0;
	int col = 0;
	char pipe, dash='-';
	int boardSize = board->n * board->m;
	char *separator = malloc(4 * boardSize + board->m + 1 + 1); /* one for extra char, one for trailing zero */
	if (separator == NULL) {memory_error("malloc");}
	pipe = '|';
	SP_BUFF_SET();
	/*init separator*/
	for (i = 0; i < (4 * boardSize + board->m + 1); i++) {
		separator[i] = dash;
	}
	separator[4 * boardSize + board->m + 1] = '\0';

	/*print each row*/
	for(row=1; row<=boardSize; row++){
		if(row%board->m == 1){
			printf("%s\n",separator);
		}

		/*print each cell in the row*/
		for(col=1; col<=boardSize; col++){
			if(col%board->n == 1){
				printf("%c", pipe);
			}
			print_cell(board, col, row, mark_errors);
		}
		printf("%c\n", pipe);
	}
	printf("%s\n",separator);

	free(separator);
}




int is_legal_set(Board *board, int col, int row, int num_to_set) {
	int i, k;
	int boardSize = board->n * board->m;
	int cell_to_check;
	int first_col_in_box = col - ((col - 1) % board->n);
	int first_row_in_box = row - ((row - 1) % board->m);
	/*check in col*/
	for (i = 1; i <= boardSize; i++) {
		if (i != row) {
			if (get_cell(board, col, i) == num_to_set) {
				return 0;
			}
		}
	}

	/*check in row*/
	for (i = 1; i <= boardSize; i++) {
		if (i != col) {
			if (get_cell(board, i, row) == num_to_set) {
				return 0;
			}
		}
	}

	/*check inside box*/
	for (i = 0; i < board->m ; i++) {
		for (k = 0; k < board->n; k++) {
			if (!(col == first_col_in_box+k && row == first_row_in_box+i)) {
				cell_to_check = get_cell(board, first_col_in_box + k, first_row_in_box + i);
				if (cell_to_check == num_to_set) {
					return 0;
				}
			}
		}
	}
	return 1;
}



Board* get_board_copy(Board *board){
	int row, col;
	int BOARD_SIZE = board->m * board->n;
	Board *copy = create_Board(board->m, board->n);
	for (row = 1; row <= BOARD_SIZE; row++) {
		for (col = 1; col <= BOARD_SIZE; col++) {
			set_cell(copy, col, row, get_cell(board,col,row));
			if(is_fixed_cell(board, col, row)){
				fix_cell(copy, col, row);
			}
		}
	}
	return copy;
}

int is_erroneous(Board *board){
	int i, j;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			if(!is_empty_cell(board, j, i) && is_error(board, j, i)){
					return 1;
			}
		}
	}
	return 0;
}

int is_finished(Board *board){
	return (get_num_empty_cells(board)==0);
}



int get_num_empty_cells(Board *board){
	int i, j, cnt=0;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			if(is_empty_cell(board, j, i)){
				cnt++;
			}
		}
	}
	return cnt;
}


void fix_all_non_empty(Board *board){
	int i, j;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			if(!is_empty_cell(board, j, i)){
				fix_cell(board, j, i);
			}
		}
	}
}

void unfix_all(Board *board){
	int i, j;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
				unfix_cell(board, j, i);
		}
	}
}







