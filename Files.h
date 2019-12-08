/*
 * Files.h
 *
 *  This module handles saving boards to filed and uploading them into the game
 *  from a given path.
 *  The module is responsible for making sure the board is legal to be saved or load, so that the
 *  flow of the game will not be harmed by illegal boards.*/


#ifndef FILES_H_
#define FILES_H_

#include "general.h"
#include <errno.h>
#include "Board.h"
#include "Messages.h"
#include "Parser.h"

#define INT_LEN 5
#define MAX_BOARD_SIZE 99



/*Save board to the location specified in path.
 * Returns 1 iff the save was done successfully*/
int save_board(Board *board, char *path);


/*Load a board from the file in filePath. If a legal board was read successfully, it is returned
 * and if not, NULL is returned.
 */
Board* load_board(char *filePath, int game_mode);

#endif /* FILES_H_ */
