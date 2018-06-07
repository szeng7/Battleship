/*Simon Zeng szeng7@jhu.edu, Bryan Ki, nki1@jhu.edu */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "commands.h"

int cntr = 0; /* global so that it prints out concede instead of last sunk */

enum result {
	EMPTY = 0, SHIP, HIT, SUNK
};

/* This initializes our board manually to the formation we want (strategized
 * beforehand w/o the code before coding it
 */

void init_board(int* board)
{
	board[11] = board[21] = SHIP;
	board[7] = board[8] = board[9] = SHIP;
	board[3] = board[13] = board[23] = board[33] = SHIP;
	board[76] = board[77] = board[78] = board[79] = SHIP;
	board[29] = board[39] = board[49] = SHIP;
	board[25] = board[26] = SHIP;
	board[52] = board[62] = SHIP;
	board[50] = board[60] = board[70] = SHIP;
	board[99] = board[98] = SHIP;
 	board[91] = board[92] = board[93] = board[94] = board[95] = SHIP;
} 

/* This is a helper function that increments the index by 11 (our diagonal
 * algorithm and then mods it by 100 and subtracts one from it if it is 
 * greater than 100. Returns incremented index.
 */

int index_increment(int index)
{
	index = index + 11;
	if (index > 99) {
		index = (index % 100) - 1;
	}
	return index;
}

/* Helper function to check if index is inbound (for when attacking blocks 
 * adjacent to corner or border blocks). Returns true if inbound, false if not
 */

bool check_inbound(int index)
{
	if ((index > 99) || (index < 0)) {
		return false;
	}
	return true;
}

/* Helper function to check if our ships were all sunk and concede needs to be
 * printed. Edits string given and returns number corresponding to certain 
 * errors and pathways in battle.c
 */

int check_concede(int cntr, char* statement) 
{
	if (cntr == 10) {
		strcpy(statement, "CONCEDE");
		return 3;
	}
	return 2;
}


/* Calculate what to say for next move and edits passed string
 */

void calc_attack(char* attack)
{
	strcpy(attack, "FIRE");
}

/* Calculate index for next attack based on previous moves and current 
 * battleboard situation. Returns index of next block to attack.
 */

int calc_index(int* opp_board, int index, char* rec_com)
{
		if (rec_com[0] == 'S') {
			opp_board[index] = SHIP;
			index = 10;
			while (opp_board[index] != EMPTY) {
				index = index_increment(index);
			}
			if(check_inbound(index) == false) {
				return 10;
			}
			return index;
		} else if (rec_com[0] == 'H') {
			opp_board[index] = SHIP;
			index = ship_hit(opp_board, index);
			if(check_inbound(index) == false) {
				return 10;
			}
			return index;			
		} else if (rec_com[0] == 'M') {
			opp_board[index] = HIT;
			index = 10;
			while (opp_board[index] == HIT) {
				index = index_increment(index);
			}
			if (opp_board[index] == SHIP) {
				index = ship_hit(opp_board, index);
			}
			if(check_inbound(index) == false) {
				return 10;
			}	
			return index;
		} else { //concede
			return -1;
		}
}
	
/* After reading FIRE command, see if opponent hit/miss/sunk/concede. Edits
 * string that will then be printed in the main function and returns an int
 * that corresponds to certain error messages and pathways in battle.c
 */

int decide_result(char* command, char col, int row, int* board, char* statement)
{
	if (strcmp(command, "CHEAT") == 0) {
		fprintf(stderr, "error: cheating detected\n");
		return 0;
	}
	if (strcmp(command, "FIRE") != 0) { 
		fprintf(stderr, "error: command given is not correct\n");
		return 0;
	}	
	
	int colNum = col - 65;
	
	if (row > 10) {
		fprintf(stderr, "error: row number chosen is out of bounds\n");
		return 0;
	}

	if ((colNum < 0) || (colNum > 9)) {
		fprintf(stderr, "error: column chosen is out of bounds\n");
		return 0;
	}

	int idx = (10 * (row - 1)) + colNum;
	
	if (board[idx] == HIT) {
		fprintf(stderr, "error: block was already hit\n");
	} else if (board[idx] == SUNK) {
		fprintf(stderr, "error: ship was already sunk\n");
	} else if (board[idx] == EMPTY) {
		board[idx] = HIT;
		strcpy(statement, "MISS");
		return 1;
	} else if (board[idx] == SHIP) {
		board[idx] = HIT;
		if ((board[11] == HIT) && (board[21] == HIT)) {
			strcpy(statement, "SUNK 2");
			board[11] = board[21] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[52] == HIT) && (board[62] == HIT)) {
			strcpy(statement, "SUNK 2");
			board[52] = board[62] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[25] == HIT) && (board[26] == HIT)) {
			strcpy(statement, "SUNK 2");
			board[25] = board[26] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[98] == HIT) && (board[99] == HIT)) {
			strcpy(statement, "SUNK 2");
			board[98] = board[99] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[7] == HIT) && (board[8] == HIT) && (board[9] == HIT)) {
			strcpy(statement, "SUNK 3");
			board[7] = board[8] = board[9] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[29] == HIT) && (board[39] == HIT) && (board[49] == HIT)) {
			strcpy(statement, "SUNK 3");
			board[29] = board[39] = board[49] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[50] == HIT) && (board[60] == HIT) && (board[70] == HIT)) {
			strcpy(statement, "SUNK 3");
			board[50] = board[60] = board[70] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[76] == HIT) && (board[77] == HIT) && (board[78] == HIT) 
				&& (board[79] == HIT)) {
			strcpy(statement, "SUNK 4");
			board[76] = board[77] = board[78] = board[79] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[3] == HIT) && (board[13] == HIT) && (board[23] == HIT)
				 && (board[33] == HIT)) {
			strcpy(statement, "SUNK 4");
			board[3] = board[13] = board[23] = board[33] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
		if ((board[91] == HIT) && (board[92] == HIT) && (board[93] == HIT)
				 && (board[94] == HIT) && (board[95] == HIT)) {
			strcpy(statement, "SUNK 5");
			board[91] = board[92] = board[93] = board[94] = board[95] = SUNK;
			cntr++;
			return check_concede(cntr, statement);
		}
			strcpy(statement, "HIT");
			return 1;
	}
	strcpy(statement, "MISS");
	return 1;
}

/* Calculates what to do when a block chosen was a ship (so basically, it 
 * goes around and hits all the blocks adjacent in a clockwise direction 
 * starting with the one to its right). Returns index of next block to attack.
 */

int ship_hit(int* opp_board, int index) {
	index = index + 1;
		if (check_inbound(index) == false) {
			index = index - 1;
		} else {
			if (opp_board[index] == EMPTY) {
				return index;
			}
			index = index - 1;
		}
	index = index + 10;
		if (check_inbound(index) == false) {	
			index = index - 10;
		} else {
			if (opp_board[index] == EMPTY) {
				return index;
			}
			index = index - 10;
		}
	index = index - 1;
		if (check_inbound(index) == false) {
			index = index + 1;
		} else {
			if (opp_board[index] == EMPTY) {
				return index;
			}		
			index = index + 1;
		}
	index = index - 10;
		if (check_inbound(index) == false) {
			index = index + 10;
		} else {
			if (opp_board[index] == EMPTY) {
				return index;
			}
			index = index + 10;
		}
	while (opp_board[index] != EMPTY) {
		index = index_increment(index);
		if (opp_board[index] == SHIP) {
			index = ship_hit(opp_board, index);
		}
	} 
	return index;

}
