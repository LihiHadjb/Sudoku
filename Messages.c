/*
 * Messages.c
 *
 *  Created on: 25 באוג׳ 2019
 *      Author: ליהי
 */
#include "Messages.h"

void autofill_changes_mssg(){
	printf("Autofill changed the following cells: \n");
}

void board_change_mssg(int col, int row, int val){
	printf("Cell %d %d was changed to %d\n", col, row, val);
}
void can_not_redo_mssg(){
	error_mssg();
	printf("There are no moves to redo.\n");
}

void can_not_undo_mssg(){
	error_mssg();
	printf("There are no moves to undo.\n");
}

void enter_command_prompt(){
	printf("Please enter a new command:\n");
}

void cant_save_mssg(){
		error_mssg();
		printf("Failed to save the desired file: ");
}

void cant_load_mssg(){
		error_mssg();
		printf("Failed to load the desired file: ");
}
void cant_close_mssg(){
	printf("Could not close the file.\n");
}


void cant_open_mssg(){
	printf("Could not open the file.\n");
}


void load_cant_close_mssg(){
	cant_load_mssg();
	cant_close_mssg();
}

void save_cant_close_mssg(){
	cant_save_mssg();
	cant_close_mssg();
}


void load_cant_open_mssg(){
	cant_load_mssg();
	cant_open_mssg();
}

void save_cant_open_mssg(){
	cant_save_mssg();
	cant_open_mssg();
}


void fixed_error_mssg(){
	cant_load_mssg();
	printf("The board contains two fixed cells that form an error.\n");
}

void no_board_mssg(){
	error_mssg();
	printf("There is no board loaded yet.\n");
}




void wrong_format_mssg(){
	cant_load_mssg();
	printf("The file does not match the legal format.\n");
}

void invalid_n_mssg(){
	cant_load_mssg();
	printf("The value of n is invalid.\n");
}

void invalid_m_mssg(){
	cant_load_mssg();
	printf("The value of m is invalid.\n");
}



void too_many_nums_mssg(){
	cant_load_mssg();
	printf("The file contains too many values.\n");
}

void not_enough_nums_mssg(){
	cant_load_mssg();
	printf("The file does not contain enough values.\n");
}

void illegal_val_for_board_mssg(){
	cant_load_mssg();
	printf("The file contains illegal values for the board.\n");
}

void empty_and_fixed_msgg(){
	cant_load_mssg();
	printf("The file contains an empty fixed cell.\n");
}

void negative_dims_mssg(){
	cant_load_mssg();
	printf("Board dimensions can not be negative.\n");

}

void too_large_board_mssg(){
	cant_load_mssg();
	printf("The board dimensions are too large.\n");

}


void cant_save_mssh(){

}

void save_open_file_error_mssg(){
	cant_save_mssg();
	printf("Could not open the file.\n");
}



void error_mssg(){
	printf("Error!\n");
}


void saved_successfully_mssg(char *path){
	printf("The board was successfully saved to: %s\n", path);
}

void can_not_save_unsolvable_mssg(){
	no_sol_mssg();
	printf("A board with no solution can not be saved in Edit mode.\n");
}


void can_not_save_erroneous_mssg(){
	erroneous_board_mssg();
	printf("An erroneous board can not be saved in Edit mode.\n");
}

void generate_error_mssg(){
	error_mssg();
	printf("Could not generate the desired puzzle!\n");
}

void erroneous_full_mssg(){
	printf("Notice!\nThe board is full but contains errors!\n");
}

void solved_mssg(){
	printf("Puzzle was solved successfully!\nUse commands 'edit' or 'solve' to load a new puzzle.\n");
}

void grb_error_mssg(){
	printf("Gurobi optimization has failed!\n");
}

void validation_succ_mssg(){
	printf("Validation succeeded. The board has a solution!\n");
}

void validation_failed_mssg(){
	printf("Validation failed. The board is not solvable!\n");
}

void invalid_threshold_mssg(){
	invalid_argument_mssg();
	printf("The threshold must be a number between 0 and 1.\n");
}

void give_guess_hint_mssg(int x, int y, double **scores, int **values, int size){
	int i;
	printf("The legal values for cell %d %d are: \n", x, y);
	for(i=0; i<size; i++){
		if((*scores)[i]>0){
			printf("%d with score %.2f.\n",(*values)[i], (*scores)[i]);
		}
	}
}
void give_hint_mssg(int x, int y, int z){
	printf("Hint: Set cell %d %d to %d.\n", x, y, z);
}

void invalid_argument_mssg(){
	printf("Invalid argument!\n");
}

void not_an_int_mssg(char* arg, int can_be_zero){
	char *rule;
	invalid_argument_mssg();
	if(!can_be_zero){
		rule = "positive";
	}
	else{
		rule = "non negative";
	}
	printf("%s argument must be a %s integer.\n", arg, rule);
}

void argument_range_mssg(int bottom, int top, char* arg, char* desc){
	invalid_argument_mssg();
	if(strcmp(desc,"") != 0){
		printf("%s. ",desc);
	}
	printf("%s argument must be an integer between %d and %d.\n", arg, bottom, top);
}



void invalid_mark_mssg(){
	argument_range_mssg(0, 1, "First", "");
}

void not_enough_empty_cells_mssg(int num_empty){
	argument_range_mssg(0, num_empty, "First","There are not enough empty cells");
}


void not_empty_cell_mssg(int x, int y){
	error_mssg();
	printf("Cell %d %d is not empty!\n", x, y);
}

void not_enough_cells_mssg(int num_cells){
	argument_range_mssg(0, num_cells, "Second","The board is too small");
}




void invalid_comm_mssg(){
	printf("%s\n","Invalid command!");
}

void invalid_len_mssg(int max_size){
	invalid_comm_mssg();
	printf("Your command is too long! A command can contain %d characters at most.\n", max_size);
}

void invalid_name_mssg(){
	invalid_comm_mssg();
	printf("There is no such command.\n");
}







void invalid_num_args_mssg(const char* comm_type, int too_many, int mands, int opts){
	char* too_many_mssg = "Too many arguments. ";
	char* missing_mssg = "Missing arguments. ";
	invalid_comm_mssg();
	if(too_many){
		printf("%s", too_many_mssg);
	}
	else{
		printf("%s", missing_mssg);
	}
	printf("A %s command has %d mandatory argument(s) and %d optional argument(s).\n", comm_type, mands, opts);
}


void too_many_args_mssg(const char* comm_type, int mands, int opts){
	invalid_num_args_mssg(comm_type, 1, mands, opts);
}


void missing_args_mssg(const char* comm_type, int mands, int opts){
	invalid_num_args_mssg(comm_type, 0, mands, opts);
}



void invalid_mode_mssg(int in_init, int in_edit, int in_solve){
	char* legal_modes;
	invalid_comm_mssg();
	if(in_init && in_edit){
		legal_modes = "Init and Edit";
	}
	else if(in_init && in_solve){
		legal_modes = "Init and Solve";
	}
	else if(in_edit && in_solve){
		legal_modes = "Edit and Solve";
	}
	else if(in_init){
		legal_modes = "Init";
	}
	else if(in_edit){
		legal_modes = "Edit";
	}
	else if(in_solve){
		legal_modes = "Solve";
	}
	printf("This command is available only in modes: %s.\n", legal_modes);
}

void no_sol_mssg(){
	error_mssg();
	printf("The board is not solvable!\n");
}


void num_sols_mssg(int num_solutions){
	printf("The board has %d solution(s).\n", num_solutions);

}
void erroneous_board_mssg(){
	error_mssg();
	printf("The board is erroneous!\n");
}


void fixed_cell_mssg(int x, int y){
	error_mssg();
	printf("Cell %d %d is fixed!\n", x, y);
}

void welcome_mssg()
{
    FILE *fptr;

    char *filename="welcome.txt", c;




    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }


    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }

    fclose(fptr);

}
