#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdbool.h>

/*Create array of "board" and set ships down*/

void init_board(int* board);

/*After reading FIRE command, see if opponent hit/miss/sunk/concede. Returns
string that will then be printed in the main function*/

int decide_result(char* command, char col, int row, int* board, char* statement);

/*While attacking, calculate move to make based on the array of the opponent's
board*/

void calc_attack(char* attack);

int calc_index(int* opp_board, int index, char* rec_com);

int ship_hit(int* opp_board, int index);

int index_increment(int index);

bool check_inbound(int index);

int check_concede(int cntr, char* statement); 

#endif /*_COMMANDS_H*/
