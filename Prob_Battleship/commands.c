/*Simon Zeng szeng7@jhu.edu*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "commands.h"

enum result 
{
	EMPTY = 0, SHIP, HIT, SUNK
};

/* Initializes the board by making it a 2D array and setting every value to 0 
*/

void init_board(struct board *board)
{
	board->r[0] = board->r0;
	board->r[1] = board->r1;
	board->r[2] = board->r2;
	board->r[3] = board->r3;
	board->r[4] = board->r4;
	board->r[5] = board->r5;
	board->r[6] = board->r6;
	board->r[7] = board->r7;
	board->r[8] = board->r8;
	board->r[9] = board->r9;
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			board->r[i][j] = 0;
		}
	}
	
	for (int k = 0; k < 10; k++) {
		board->ships[k] = 0;
	}
}

/* Initializes the board by making it a 2D array and setting every value to 0.0
 */

void init_float_board(struct float_board *prob_board)
{
	prob_board->r[0] = prob_board->r0;
	prob_board->r[1] = prob_board->r1;
	prob_board->r[2] = prob_board->r2;
	prob_board->r[3] = prob_board->r3;
	prob_board->r[4] = prob_board->r4;
	prob_board->r[5] = prob_board->r5;
	prob_board->r[6] = prob_board->r6;
	prob_board->r[7] = prob_board->r7;
	prob_board->r[8] = prob_board->r8;
	prob_board->r[9] = prob_board->r9;
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			prob_board->r[i][j] = 0.0;
		}
	}
	
	for (int k = 0; k < 10; k++) {
		prob_board->ships[k] = 0;
	}
}

/* Sets up the ships on the board in the formation planned beforehand 
*/

void set_ships(struct board *board)
{	
	board->r[1][1] = board->r[2][1] = SHIP;
	board->r[2][5] = board->r[2][6] = SHIP;
	board->r[5][2] = board->r[6][2] = SHIP;
	board->r[9][9] = board->r[9][8] = SHIP;
	board->r[0][7] = board->r[0][8] = board->r[0][9] = SHIP;
	board->r[2][9] = board->r[3][9] = board->r[4][9] = SHIP;
	board->r[5][0] = board->r[6][0] = board->r[7][0] = SHIP;
	board->r[0][3] = board->r[1][3] = board->r[2][3] = board->r[3][3] = SHIP;
	board->r[7][6] = board->r[7][7] = board->r[7][8] = board->r[7][9] = SHIP;
	board->r[9][1] = board->r[9][2] = board->r[9][3] = board->r[9][4] = board->r[9][5] = SHIP;	
	
}

/* Takes in the "attack" from the opponent and processes whether the opponent
 * hit, miss, sunk or conceded. Edits the "statement" given to it by battle.c 
 * based on the result (to be printed out by battle.c). Outputs 0 if we win
 * (opponent concedes, attacks out of bounds, or gives incorrect command).
 */

int decide_result(char* statement, int row, char col, struct board *own_board)
{
	if (strcmp(statement, "CONCEDE") == 0 || 
			col > 'J' || col < 'A' || row > 10 || row < 0) {
		return 0;
	} //this means the opponent conceded or attacked "out of bounds"
	
	if (strcmp(statement, "FIRE") != 0) {
		return 0;
	}

	/* If the statement gets past the first two conditionals, then
	 * it's a fire statement */
	
	int col_num = col - 65;	
	row = row - 1;
	bool ship_sank;

	if (own_board->r[row][col_num] == EMPTY ||
			own_board->r[row][col_num] == SUNK ||
			own_board->r[row][col_num] == HIT) {
		//if spot is hit again, it "misses"
		strcpy(statement, "MISS");
		return 1;	
	} else if (own_board->r[row][col_num] == SHIP) {
		own_board->r[row][col_num] = HIT;
		ship_sank = check_ship_sunk(own_board, statement);
		if (ship_sank == true) {
			if (check_concede(own_board) == true) {
				strcpy(statement, "CONCEDE");
				return 1;
			}
			return 1;
		} else {
			strcpy(statement, "HIT");
			return 1;
		}
	} else {
		fprintf(stderr, "error: check decide_result function\n");
		return 0;
	}
}

/*Helper function to check if a ship was sunk by opponent 
 * Returns true if ship was sunk */

bool check_ship_sunk(struct board *board, char* statement)
{
	if (board->r[1][1] == HIT && board->r[2][1] == HIT) {
		board->r[1][1] = board->r[2][1] = SUNK;
		board->ships[0] = 1;
		strcpy(statement, "SUNK 2");
		return true;	
	}
	
	if (board->r[2][5] == HIT && board->r[2][6] == HIT) {
		board->r[2][5] = board->r[2][6] = SUNK;
		board->ships[1] = 1;
		strcpy(statement, "SUNK 2");
		return true;
	}
	
	if (board->r[5][2] == HIT && board->r[6][2] == HIT) {
		board->r[5][2] = board->r[6][2] = SUNK;
		board->ships[2] = 1;
		strcpy(statement, "SUNK 2");
		return true;
	}
	
	if (board->r[9][9] == HIT && board->r[9][8] == HIT) {
		board->r[9][9] = board->r[9][8] = SUNK;
		board->ships[3] = 1;
		strcpy(statement, "SUNK 2");
		return true;
	}
	
	if (board->r[0][7] == HIT && board->r[0][8] == HIT && board->r[0][9] == HIT) {
		board->r[0][7] = board->r[0][8] = board->r[0][9] = SUNK;
		board->ships[4] = 1;
		strcpy(statement, "SUNK 3");
		return true;
	}
	
	if (board->r[2][9] == HIT && board->r[3][9] == HIT && board->r[4][9] == HIT) {
		board->r[2][9] = board->r[3][9] = board->r[4][9] = SUNK;
		board->ships[5] = 1;
		strcpy(statement, "SUNK 3");
		return true;
	}
	
	if (board->r[5][0] == HIT && board->r[6][0] == HIT && board->r[7][0] == HIT) {
		board->r[5][0] = board->r[6][0] = board->r[7][0] = SUNK;
		board->ships[6] = 1;
		strcpy(statement, "SUNK 3");	
		return true;
	}
	
	if (board->r[0][3] == HIT && board->r[1][3] == HIT && 
			board->r[2][3] == HIT && board->r[3][3] == HIT) {
		board->r[0][3] = board->r[1][3] = board->r[2][3] = board->r[3][3] = SUNK;
		board->ships[7] = 1;
		strcpy(statement, "SUNK 4");
		return true;
	}
	
	if (board->r[7][6] == HIT && board->r[7][7] == HIT && 
			board->r[7][8] == HIT && board->r[7][9] == HIT) {
		board->r[7][6] = board->r[7][7] = board->r[7][8] = board->r[7][9] = SUNK;
		board->ships[8] = 1;
		strcpy(statement, "SUNK 4");
		return true;
	}

   	if (board->r[9][1] == HIT && board->r[9][2] == HIT && 
			board->r[9][3] == HIT && board->r[9][4] == HIT && 
			board->r[9][5] == HIT) {
		
		board->r[9][1] = board->r[9][2] = board->r[9][3] 
			= board->r[9][4] = board->r[9][5] = SUNK;
		board->ships[9] = 1;	
		strcpy(statement, "SUNK 5");
		return true;
	}

	return false;
}

/* Helper function to see if all ships have been sunk 
 * returns true if yes */

bool check_concede(struct board *board) 
{
	for (int i = 0; i < 10; i++) {
		if (board->ships[i] == 0) {
			return false;
		}
	}
	board->concede = 1;
	return true;
}

/* Updates our representation of the opponent's board based on 
 * the results of our attacks   
 * returns target to be pushed onto stack */

int update_opp_board(char* result_of_attack, struct board *opp_board, 
		int row, char col, int ship_size) 
{
	int col_num = col - 65; //8
	int orig_row = row - 1; //10
	int orig_col = col_num;
	row = row - 1;
	if (strcmp(result_of_attack, "MISS") == 0) {
		opp_board->r[row][col_num] = HIT;
		return -1;
	} else if (strcmp(result_of_attack, "HIT") == 0) {
		opp_board->r[row][col_num] = SHIP;
		return row*10 + col_num;
	} else { 
		opp_board->r[row][col_num] = SUNK;
		mark_sunk_ship(opp_board, ship_size);
		//This next segment marks off the previously "SHIP" blocks as "SUNK"
		//check up
		int ship_marked = 0; //This stops this branch when ship is marked sunk

		if ((row-ship_size+1) >= 0 && ship_marked == 0) {
			for (int i = 0; i < ship_size-1; i++) {
				if (opp_board->r[row-1][col_num] == SHIP) {
			    	opp_board->r[row-1][col_num] = SUNK;
		   	 		row = row - 1;
				} else {
					while (i != 0) {
						if (opp_board->r[row+1][col_num] == SUNK) {
							opp_board->r[row+1][col_num] = SHIP;
							row = row + 1;
						}
						i = i - 1;
					}
					break;
				}
			ship_marked = 1;
			}	
		}
		row = orig_row;
		col_num = orig_col;
		//check right
		if ((col_num+ship_size-1) <= 9 && ship_marked == 0) {	
			for (int i = 0; i < ship_size-1; i++) {
				if (opp_board->r[row][col_num+1] == SHIP) {
			    	opp_board->r[row][col_num+1] = SUNK;
		   	 		col_num = col_num + 1;
				} else {
					while (i != 0) {
						if (opp_board->r[row][col_num-1] == SUNK) {
							opp_board->r[row][col_num-1] = SHIP;
							col_num = col_num - 1;
						}
						i = i - 1;
					}
					break;
				}
			ship_marked = 1;
			}
		}
		row = orig_row;
		col_num = orig_col;	
		//check down
		if ((row+ship_size-1) <= 9 && ship_marked == 0) {	
			for (int i = 0; i < ship_size-1; i++) {
				if (opp_board->r[row+1][col_num] == SHIP) {
			    	opp_board->r[row+1][col_num] = SUNK;
		   	 		row = row + 1;
				} else {
					while (i != 0) {
						if (opp_board->r[row-1][col_num] == SUNK) {
							opp_board->r[row-1][col_num] = SHIP;
							row = row - 1;
						}
						i = i - 1;
					}
				break;
				}
			ship_marked = 1;
			}
		}
		row = orig_row;
		col_num = orig_col;
	//check left
		if ((col_num-ship_size+1) >= 0 && ship_marked == 0) {
			for (int i = 0; i < ship_size-1; i++) {
				if (opp_board->r[row][col_num-1] == SHIP) {
			    	opp_board->r[row][col_num-1] = SUNK;
		   	 		col_num = col_num - 1;
				} else {
					while (i != 0) {
						if (opp_board->r[row][col_num+1] == SUNK) {
							opp_board->r[row][col_num+1] = SHIP;
							col_num = col_num + 1;
						}
						i = i - 1;
					}
					break;
				}
			ship_marked = 1;
			}
		}
		row = orig_row;
		col_num = orig_col;	
	}
	return 0;
}
/* Helper function to mark the ship as sunk in the "ship array" of the board */

void mark_sunk_ship(struct board *opp_board, int ship_size) {
	switch(ship_size) {
		case 2: 
			for (int a = 0; a < 4; a++) {
				if (opp_board->ships[a] == 0) {
					opp_board->ships[a] = 1;
					break;
				}
			} break;
		case 3: 
			for (int b = 4; b < 7; b++) {
				if (opp_board->ships[b] == 0) {
					opp_board->ships[b] = 1;
					break;
				}
			} break;
		case 4:
			for (int c = 7; c < 9; c++) {
				if (opp_board->ships[c] == 0) {
					opp_board->ships[c] = 1;
					break;
				}
			} break;
		case 5:
			if (opp_board->ships[9] == 0) {
				opp_board->ships[9] = 1;
			} break;
		default:
			fprintf(stderr, "error: ship size given is invalid\n");
			break;
	}
}

/*Updates the prob_board based on the new updates to the opp_board. The values
 * are based on how many of the remaining ships are able to be in the spot */

void update_prob_board(struct board *opp_board, struct float_board *prob_board) {
	
	int total_ships = 0;
  	int carriers = 0;
    int	battleships = 0;
    int	cruisers = 0;
    int	destroyers = 0;

	for (int a = 0; a < 10; a++) {
		if (opp_board->ships[a] == 0) {
			total_ships++;
			if (a < 4) {
				destroyers++;
			} else if ((3 < a) && (a < 7)) {
				cruisers++;
			} else if ((6 < a) && (a < 9)) {
				battleships++;
			} else {
				carriers++;
			}
		}
	}
	int config = 0;
	int spaces = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (opp_board->r[i][j] == EMPTY) { //"Haven't tried to hit yet"
				//this next segment will check how many vertical spaces there are
				int orig_i = i;
				int orig_j = j;
				while (i >= 0) {
					if ((opp_board->r[i][j] != HIT) &&
							(opp_board->r[i][j] != SUNK)) {
						spaces++;
						i = i-1;
					} else {
						break;
					}
				}
				
				if (spaces > 1) { config = config + destroyers; }
				if (spaces > 2) { config = config + cruisers; }
				if (spaces > 3) { config = config + battleships; }
				if (spaces > 4) { config = config + carriers; }
				
				i = orig_i;
				spaces = 0;

				while (i < 10) {
					if ((opp_board->r[i][j] != HIT) && 
							(opp_board->r[i][j] != SUNK)) {
						spaces++;
						i = i+1;
					} else {
						break;
					}
				}

				if (spaces > 1) { config = config + destroyers; }
				if (spaces > 2) { config = config + cruisers; }
				if (spaces > 3) { config = config + battleships; }
				if (spaces > 4) { config = config + carriers; }

				i = orig_i;
				spaces = 0;
				
				while (j >= 0) {
					if ((opp_board->r[i][j] != HIT) && 
							(opp_board->r[i][j] != SUNK)) {
						spaces++;
						j = j-1;
					} else {
						break;
					}
				}

				if (spaces > 1) { config = config + destroyers; }
				if (spaces > 2) { config = config + cruisers; }
				if (spaces > 3) { config = config + battleships; }
				if (spaces > 4) { config = config + carriers; }
		
				j = orig_j;
				spaces = 0;

				while (j < 10) {
					if ((opp_board->r[i][j] != HIT) && 
							(opp_board->r[i][j] != SUNK)) {
						spaces++;
						j = j+1;
					} else {
						break;
					}
				}

				if (spaces > 1) { config = config + destroyers; }
				if (spaces > 2) { config = config + cruisers; }
				if (spaces > 3) { config = config + battleships; }
				if (spaces > 4) { config = config + carriers; }

				j = orig_j;
				i = orig_i;
			} else {
				/* if the spot isn't empty, then that means we hit the spot already.
				 * therefore, there's no point in hitting it again, so prob = 0 */
				config = 0;
			}
			prob_board->r[i][j] = (float) config / (4 * total_ships);
			spaces = 0;
			config = 0;
		}
	}
}
/* Looks at the probability values of the prob_board and chooses the 
 * highest one as the next block to attack */

int prob_attack(char* statement, struct float_board *prob_board, struct board *opp_board) {
	int target = 0;
	float max = prob_board->r[0][0];
	strcpy(statement, "FIRE");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (prob_board->r[i][j] > max) {
		   			max = prob_board->r[i][j];
		   			target = (i*10)+j;
				}
			}
		}
	return target;
}

/*Helper function to check if index, row and column of block is in bound*/

bool inbound(int index, int row, int col)
{
	if (index < 100 && index >= 0) {
		if (row < 11 && row >= 0) {
			if (col < 11 && col >= 0) {
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

/* Only used if the previous block hit was a ship: this function has the program
 * attack the blocks surrounding the previous one (going clockwise). If a
 * direction is more likely to contain a block (so if to the left is a ship, then
 * it is very likely that to the right is also a ship), then the function
 * chooses that block as a target first */

int target_attack(char* statement, int target, struct board *opp_board) {

	strcpy(statement, "FIRE");	
	
	int col = target % 10;
	int orig_col = col;

	int row = (target - col) / 10;
	int orig_row = row;

	int orig_target = target;

	if (inbound(target-1, row, col-1) && opp_board->r[row][col-1] == SHIP) {
		target = target + 1;
		if (inbound(target, row, col+1) && opp_board->r[row][col+1] == EMPTY) {
			return target;
		}
		target = orig_target;
	}
	
	if (inbound(target+1, row, col+1) && opp_board->r[row][col+1] == SHIP) {
		target = target - 1;
		if (inbound(target, row, col-1) && opp_board->r[row][col-1] == EMPTY) {
			return target;
		}
		target = orig_target;
	}

	if (inbound(target+10, row+1, col) && opp_board->r[row+1][col] == SHIP) {
		target = target - 10;
		if (inbound(target, row-1, col) && opp_board->r[row-1][col] == EMPTY) {
			return target;
		}
		target = orig_target;
	}

	if (inbound(target-10, row-1, col) && opp_board->r[row-1][col] == SHIP) {
		target = target + 10;
		if (inbound(target, row+1, col) && opp_board->r[row+1][col] == EMPTY) {
			return target;
		}
		target = orig_target;
	}

	target = target + 1;

	if (inbound(target, row, col+1) && opp_board->r[row][col+1] == EMPTY) {
		return target;
	}
		
	target = orig_target;
	target = target + 10;
		
	if (inbound(target, row+1, col) && opp_board->r[row+1][col] == EMPTY) {
		return target;
	}

	target = orig_target;
	target = target - 1;

	if (inbound(target, row, col-1) && opp_board->r[row][col-1] == EMPTY) {
		return target;
	}
		
	target = orig_target;
	target = target - 10;
		
	if (inbound(target, row-1, col) && opp_board->r[row-1][col] == EMPTY) {
		return target;
	}
		
	return 0;

}


