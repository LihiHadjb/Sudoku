/*
 * Files.c
 *
 *  Created on: 17 בספט׳ 2019
 *      Author: ליהי
 */


#include "Files.h"


/*Return 1 iff the value in cell (check_col, check_row) is fixed and therefore forms an error
 * of 2 fixed cells with the value in this_val (which is assumed to be the values of another
 * fixed cell).
 */
/*int is_fixed_error(Board *board, int this_val, int check_col, int check_row){
	int other_val;
	other_val = get_cell(board, check_col, check_row);
	if(is_fixed_cell(board, check_col, check_row) && other_val == this_val){
		return 1;
	}
	return 0;
}*/

/*return 1 iff cell (col, row) forms an error of two fixed cells with some other cell on
* the same row (assumes cell (col, row) is fixed)*/
/*int has_fixed_error_row(Board *board, int col,int row){
	int i, this_val;
	int board_size = board->m * board->n;
	this_val = get_cell(board, col, row);
	for(i=1; i<=board_size; i++){
		if(i!=col){
			if(is_fixed_error(board, this_val,  i,  row)){
				return 1;
			}
		}
	}
	return 0;
}*/


/*return 1 iff cell (col, row) forms an error of two fixed cells with some other cell on
* the same column (assumes cell (col, row) is fixed)*/
/*int has_fixed_error_col(Board *board, int col,int row){
	int i, this_val;
	int board_size = board->m * board->n;
	this_val = get_cell(board, col, row);
	for(i=1; i<=board_size; i++){
		if(i!=row && is_fixed_cell(board, col, i)){
			if(is_fixed_error(board, this_val,  col,  i)){
				return 1;
			}
		}
	}
	return 0;
}*/

/*Return 1 iff cell (col, row) forms an error of two fixed cells with some other cell on
* the same block (assumes cell (col, row) is fixed)*/
/*int has_fixed_error_block(Board *board, int col,int row){
	int first_col_in_box = first_col_in_box(board, col);
	int first_row_in_box = first_row_in_box(board, row);
	int check_col, check_row;
	int i,k, this_val;
	this_val = get_cell(board, col, row);*/

	/*for each cell in the block of cell (col,row), check if these two cells from an
	 * error of 2 fixed cells*/
	/*for (i = 0; i < board->m ; i++) {
		for (k = 0; k < board->n; k++) {
			check_col = first_col_in_box+k;
			check_row = first_row_in_box+i;
			if (!(col == check_col && row == check_row)) {
				if(is_fixed_error(board, this_val,  check_col,  check_row)){
					return 1;
				}

			}
		}
	}
	return 0;
}
*/


/*Return 1 iff the board contains 2 fixed cells that are neighbours and have the
 * same value.
 */
/*
int exist_fixed_error(Board *board){
	int i, j;
	int board_size = board->n * board->m;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			if(is_fixed_cell(board, j, i)){
				if(has_fixed_error_row(board, j,i) || has_fixed_error_col(board, j,i) || has_fixed_error_block(board, j,i)){
					fixed_error_mssg();
					return 1;
				}
			}

		}
	}
	return 0;

}*/


/*Remove from board all the values in cells that are not fixed. This is and auxiliary functions
 * for has_fixed_error*/
void clean_all_unfixed(Board *board){
	int i, j;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
			for(j=1; j<=board_size; j++){
				if(!is_fixed_cell(board, j, i)){
					clean_cell(board, j, i);
				}
			}
	}
}


/*Return 1 iff the board contains 2 fixed cells that are neighbors and have the
 * same value (i.e. they form an error of 2 fixed cells).
 */
int has_fixed_error(Board *board){
	Board *board_copy = get_board_copy(board);
	clean_all_unfixed(board_copy);
	if( is_erroneous(board_copy)){
		fixed_error_mssg();
		destroy_Board(board_copy);
		return 1;
	}
	destroy_Board(board_copy);
	return 0;

}











/*Reads the next value of a cell from the file, and put it in val. Checks also if this
 * cell is marked as fixed, and puts that information in is_fixed.
 * Returns 1 iff an integer (with or without fixed cell symbol) was read and parsed succesfully*/
int read_next_val(FILE *file, int *val, int *is_fixed){
	char val_str[INT_LEN] = {0};
	char *end_ptr;
	int temp;

	/*parse the next number*/
	if (fscanf(file, "%4s", val_str) != 1 || val_str[INT_LEN - 2] != '\0') {
		wrong_format_mssg();
		return 0;
	}

	/*convert the string to an integer*/
	temp = strtol(val_str, &end_ptr, 10);
	if(!(val_str != end_ptr)){
		wrong_format_mssg();
		return 0;
	}

	/*fix the cell if neccesary*/
	if(*end_ptr == '\0'){
		*is_fixed=0;
		*val = temp;
	}

	else if(strcmp(end_ptr, ".") == 0){
		*is_fixed = 1;
		*val = temp;
	}

	else{
		return 0;
	}

	return 1;
}


/*Return 1 iff the value in val fits the board sizes and rules and can be assigned to a cell.*/
int check_val(Board *board, int val, int is_fixed){
	int board_size = board->m * board->n;

	/*check if val fits the board sizes*/
	if(val>board_size || val<0){
		illegal_val_for_board_mssg();
		return 0;
	}

	/*an empty cell can not be fixed*/
	if(val==0 && is_fixed){
		empty_and_fixed_msgg();
		return 0;
	}
	return 1;
}


/*Return 1 iff the board dimensions read from the file are legal*/
int check_dims(int m, int n){
	if(m<0 || n<0){
		negative_dims_mssg();
		return 0;
	}
	if(m*n>MAX_BOARD_SIZE){
		too_large_board_mssg();
		return 0;
	}
	return 1;
}

/*Returns 1 iff a legal board was read successfully from the file*/
int read_board(FILE *file, Board *board){
	int tempCell=0, is_fixed=0;
	int i, j;
	int board_size = board->m * board->n;
	for(i=1; i<=board_size; i++){
		for(j=1; j<=board_size; j++){
			/*read a value for each cell and set the relevant
			 *cell on the board if it is*/
			if(!read_next_val(file, &tempCell, &is_fixed)){
				return 0;
			}

			if(!check_val(board, tempCell, is_fixed)){
				return 0;
			}

			set_cell(board, j, i, tempCell);
			if(is_fixed){
				fix_cell(board, j, i);
			}
		}
	}

	return 1;
}


/*Return 1 iff a board dimension was read successfully from the file*/
int read_next_dim(FILE *file, int *res_num){
	char res_str[3]="\0\0\0";/*check!!!*/
	char *end_ptr;
	int num;

	/*read a token*/
	if(fscanf(file, "%4s",res_str) != 1){
			return 0;
	}

	/*convert to an int*/
	num = strtol(res_str, &end_ptr, 10);
	if(!(num >= 0 && res_str != end_ptr)){
		return 0;
	}

	/*dimensions can not be fixed*/
	if((strcmp(end_ptr, ".")==0)){
		return 0;
	}

	*res_num = num;
	return 1;
}


/*Return 1 iff the board dimensions were read successfully from the file, and puts these
 * values in m and n.*/
int get_dims(FILE *file, int *m, int *n){
	if(!read_next_dim(file, m)){
		invalid_m_mssg();
		return 0;
	}
	if(!read_next_dim(file, n)){
		invalid_n_mssg();
		return 0;
	}
	if(!check_dims(*m, *n)){
		return 0;
	}
	return 1;
}

/*Returns 1 if all of the tokens in the file were already read*/
int reached_end(FILE *file){
	char word[INT_LEN];
    if (fscanf(file, "%4s", word) == 1){
    	too_many_nums_mssg();
        return 0;
    }
	return 1;
}


Board* load_board(char *filePath, int game_mode) {
	Board *board=(Board*)malloc(sizeof(Board));
	int result=1;
	int n=0, m=0;
	FILE *file;
	SP_BUFF_SET();

	/*open the file to be read*/
	if ((file = fopen(filePath, "r")) == NULL) {
		load_cant_open_mssg();
		return NULL;
	}
	/*parse m and n*/
	else if(!get_dims(file, &m, &n)){
		if(fclose(file) == EOF){
			load_cant_close_mssg();
		}
		return NULL;
	}
	board = create_Board(m, n);
	/*fill the values in the board*/
	if(!read_board(file, board)){
		result = 0;
	}
	/*check if there are too many values*/
	else if(!reached_end(file)){
		result = 0;
	}
	/*check if there are 2 fixed cells which form an error*/
	else if(game_mode == MODE_SOLVE && has_fixed_error(board)){
		result =0;
	}
	/*When loading in edit mode, there are no fixed cells*/
	else if(game_mode == MODE_EDIT){
		unfix_all(board);
	}
	/*close the file*/
	if(fclose(file) == EOF){
		load_cant_close_mssg();
		result = 0;
	}

	if(result == 0){
		destroy_Board(board);
		return NULL;
	}
	return board;
}


int save_board(Board *board, char *path){
	int i, j;
	FILE* file_ptr;
	int cell_value;
	char space = ' ';
	char fixed_symbol = '.';
	char empty_symbol = '0';
	int blockSize = board->n * board->m;

	/*open the file*/
	if((file_ptr = fopen(path, "w")) == NULL){
		save_cant_open_mssg();
		free(path);
		return 0;
	}
	SP_BUFF_SET();

	/*write board dims to the file*/
	fprintf(file_ptr, "%d%c%d\n", board->m, space, board->n);

	/*write each cell to the file*/
	for (i = 1; i <= blockSize; i++) {
		for (j = 1; j <= blockSize; j++) {
			cell_value = get_cell(board,  j, i);

			if (is_empty_cell(board, j, i)) {
				fprintf(file_ptr, "%c",empty_symbol);
			}
			else{
				fprintf(file_ptr, "%d",cell_value);
			}

			if (is_fixed_cell(board, j, i)) {
				fprintf(file_ptr, "%c",fixed_symbol);
			}

			if(j!=blockSize){
				fprintf(file_ptr, "%c",space);
			}
		}
		fprintf(file_ptr,"\n");
	}

	/*close the file*/
	if(fclose(file_ptr) == EOF){
		save_cant_close_mssg();
		free(path);
		return 0;
	}
	return 1;
}



