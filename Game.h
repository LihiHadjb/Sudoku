/*
 * Game.h
 *
 * This module implements all the game commands and the methods responsible for the flow of the game
 * Here we also check the validity of the user's commands in terms of compatibility to the current board
 * and its state
 * The game module receives a command, and calls the other modules in order to execute it. It
 * also produces messaged to the user by using the Messages module.
 **/




#ifndef GAME_H_
#define GAME_H_


#include "general.h"
#include "Parser.h"
#include "Messages.h"
#include "Board.h"
#include "Exhaust_Backtrack.h"
#include "GRB_Solver.h"
#include "Options_Board.h"
#include "Random.h"
#include "Puzzle_Generator.h"
#include "Files.h"
#include "Memory.h"



typedef struct Game{
	struct Board *curr_board;
	/*struct Board *solution;*/
	int is_solved;/*remove!!!!*/
	int mode;
	int mark_errors;
	int board_initialized;
	Memory *mem;

}Game;

#define DEFAULT_M 3
#define DEFAULT_N 3

/*As described in the project's instructions*/
int generate(Game *game, int x, int y);

/*Return 1 iff errors should be marked when printing the board*/
int get_mark_errors(Game* game);

/*create a Game instance and initialize it*/
Game* create_Game();

/*This function is responsible for the flow of the game. It operates a loop that gets commands from the user*/
/*All game commands return 1 if the command succeded and the program should wait for another command.
If the game should be stopped ('exit'), we return -1, and 0 in all other cases*/
int manage_game(Game *game);


/*Initialize a game with cells_to_fill numbers on board*/
int init_game();

/*Delete a board instance and free all relevant memory*/
void destory_Board(Board board);

/*Delete a game instance and free all relevant memory*/
void destroy_Game();


/*redo the steps memory list and revert the board to its original state*/
int restart_game();


/*call the command specified in comm with the parsed arguments. This function assumes the argument
 * are valid for the command and that their number is as expected*/
/*return 1 iff the operation succeeded*/
int exec_command(Game *game, Command command);

/*As described in 'solve' in the project's instruction*/
int solve_comm(Game *game, char* x);



/*As described in 'edit' in the project's instruction*/
int edit_comm(Game *game, char* x);

/*As described in 'mark errors' in the project's instruction*/
int mark_errors(Game *game, int x);

/*print to the user the current board state*/
int print_board_comm(Game *game);

/*Place num_to_set value that the user gives in cell (x,y) on the game's board*/
/*return 1 if update succeeded and 0 otherwise*/
int set(Game *game, int x, int y, int z);

/*Check if the current board is solvable and give the user an update message, using ILP*/
int validate(Game *game);

/*solve the board with LP and fill legal values that got a score>threshold*/
int guess (Game *game, float threshold);

/*undo the last step done by the user*/
int undo(Game *game);

/*rredo the last step done by the user*/
int redo(Game *game);

/*As described in 'save' in the project's instruction*/
int save(Game *game, char* x);

/*Give the user a possible value for cell (col,row), using ILP to solve the board*/
int hint(Game *game,int x, int y);

/*As described in 'guess_hint' in the project's instruction*/
int guess_hint(Game *game, int x, int y);
/*Return the number of solutions the current board has, using the exhaustive backtracking algorithm*/
int num_solutions(Game *game);

/*fill all cells on the current board of game that have only one possible legal assignment*/
/*return 1 iff the operation succeeded*/
int autofill(Game *game);

/*As described in 'reset' in the project's instruction*/
int reset(Game *game);

/*Terminate the game and free all relevant memory*/
int exit_game(Game *game);

int remove_illegal_values(Board *board, double **scores, int **values, int num_elems, int col, int row);






#endif /* GAME_H_ */
