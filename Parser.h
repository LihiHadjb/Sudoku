/*
 * Parser.h
 *This module is responsible for handling game commands entered by the user. It provides the Game
 *module a valid command, that has the correct number of arguments with correct ranges, that can be
 *executed in the current game mode and board dimensions.
 *Each command has a code defined below, to ease handling with the command string*/



#ifndef PARSER_H_
#define PARSER_H_

#include "general.h"
#include "Messages.h"
#include <errno.h>

#define DELIM " "
#define MAX_COMMAND_SIZE 256
#define NUM_TYPES 17

#define SOLVE 1
#define EDIT 2
#define MARK_ERRORS 3
#define PRINT_BOARD 4
#define SET 5
#define VALIDATE 6
#define GUESS 7
#define GENERATE 8
#define UNDO 9
#define REDO 10
#define SAVE 11
#define HINT 12
#define GUESS_HINT 13
#define NUM_SOLUTIONS 14
#define AUTOFILL 15
#define RESET 16
#define EXIT 17

#define MODE_INIT 1
#define MODE_EDIT 2
#define MODE_SOLVE 3

typedef char* Command;




/*Prompts for a new command from the user and checks its validity with respect to the mode in curr_mode,
 * and a board with dims m, n and num_empty empty_cells.
 * Returns a valid command (i.e correct name, number of arguments, their ranges and types, that is
 * available in the specified mode*/
Command get_command_from_user(int curr_mode, int m, int n, int num_empty);

/*Returns the type code of the command in comm_str, according to the name of the command. If no
 * such command exists, returns 0;*/
 int get_type_code(const Command comm_str);


/*Returns the first argument of the command in comm_str as a string to be used later as a file path.*/
char* parse_path(const Command comm_str);

/*Returns the first argument of the command in comm_str as an int.
 * If the parsing failed (for example if the argument is not a int or is not a number) returns -1.*/
int parse_x(const Command comm_str);

/*Returns the second argument of the command in comm_str as an int.
 * If the parsing failed (for example if the argument is not a int or is not a number) returns -1.*/
int parse_y(const Command comm_str);

/*Returns the third argument of the command in comm_str as an int.
 * If the parsing failed (for example if the argument is not a int or is not a number) returns -1.*/
int parse_z(const Command comm_str);

/*Returns the first argument of the command in comm_str as a float, to be used later as a threshold
 * for the guess command.
 *If the parsing failed (for example if the argument is not a number) returns -1.*/
float parse_guess(const Command comm_str);



#endif /* PARSER_H_ */
