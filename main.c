/*
 * main.c
 *
 *  Created on: 22 באוג׳ 2019
 *      Author: ליהי
 */


/*#include "Game.h"*/
/*#include "Exhaust_Backtrack.h"*/
/*#include "test.h"*/
#include "Board.h"
#include "GRB_Solver.h"
#include "Random.h"
#include "Exhaust_Backtrack.h"
#include "Game.h"

int main(){
	Game *g=create_Game();
	manage_game(g);

	return 1;
}
