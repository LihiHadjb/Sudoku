/*
 * Messages.h
 *
 *  This module is responsible for giving the user messages regarding the game and notify him
 *  if the command succedded or instruct him what to do.
 *  The idea behind putting all the messages of the entire game in one module is to create similiar
 *  templated of messages (for instance, errors) and to separate defining the exact text of a
 *  message from the logic of the code.
 *  The functions names are descriptive thus they are not documented*/


#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "general.h"
void autofill_changes_mssg();
void board_change_mssg(int col, int row, int val);
void can_not_undo_mssg();
void can_not_redo_mssg();
void enter_command_prompt();
void cant_save_mssg();

void cant_load_mssg();
void cant_close_mssg();


void cant_open_mssg();

void load_cant_close_mssg();

void save_cant_close_mssg();


void load_cant_open_mssg();

void save_cant_open_mssg();




void fixed_error_mssg();
void no_board_mssg();
void wrong_format_mssg();
void error_mssg();


void invalid_n_mssg();

void invalid_m_mssg();


void too_many_nums_mssg();

void not_enough_nums_mssg();


void illegal_val_for_board_mssg();

void empty_and_fixed_msgg();

void negative_dims_mssg();

void too_large_board_mssg();




void saved_successfully_mssg(char *x);
void can_not_save_erroneous_mssg();
void can_not_save_unsolvable_mssg();
void not_an_int_mssg(char* arg, int can_be_zero);
void generate_error_mssg();
void erroneous_full_mssg();
void solved_mssg();
void grb_error_mssg();
void validation_failed_mssg();
void validation_succ_mssg();
void invalid_threshold_mssg();
void give_guess_hint_mssg(int x, int y, double **scores, int **values, int size);
void give_hint_mssg(int x, int y, int z);
void not_empty_cell_mssg(int x, int y);
void fixed_cell_mssg(int x, int y);
void no_sol_mssg();
void invalid_argument_mssg();
void argument_range_mssg(int bottom, int top, char* arg, char* desc);
void invalid_mark_mssg();
void not_enough_empty_cells_mssg(int num_empty);
void not_enough_cells_mssg(int num_cells);
void welcome_mssg();


void too_many_args_mssg(const char* comm_type, int mands, int opts);
void missing_args_mssg(const char* comm_type, int mands, int opts);
void invalid_name_mssg();
void invalid_len_mssg(int max_size);
void invalid_mode_mssg(int in_init, int in_edit, int in_solve);
void num_sols_mssg(int num_solutions);
void erroneous_board_mssg();


#endif /* MESSAGES_H_ */
