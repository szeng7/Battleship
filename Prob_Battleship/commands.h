#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdbool.h>

struct board
{
	int concede;
	int r0[10];
	int r1[10];
	int r2[10];
	int r3[10];
	int r4[10];
	int r5[10];
	int r6[10];
	int r7[10];
	int r8[10];
	int r9[10];
	int* r[10];
	int ships[10]; //this keeps track of sunk ships (1 means sunk, 0 means not sunk)
};

struct float_board
{
	int concede;
	float r0[10];
	float r1[10];
	float r2[10];
	float r3[10];
	float r4[10];
	float r5[10];
	float r6[10];
	float r7[10];
	float r8[10];
	float r9[10];
	float* r[10];
	int ships[10];
};
/*Initializes the board*/
void init_board(struct board *board);

/*Initializes the board*/
void init_float_board(struct float_board *prob_board);

/*Changes elements of the board's 2D array to represent "ships"*/
void set_ships(struct board *board);

/*Determines the result of the opponent's attack*/
int decide_result(char* statement, int row, char col, struct board *own_board);

/*Updates our representation of opp's board based on our attacks*/
int update_opp_board(char* result_of_attack, struct board *opp_board, 
		int row, char col, int ship_size);

/*Updates prob_board based on updates to the opp_board*/
void update_prob_board(struct board *opp_board, struct float_board *prob_board);

/*Checks if the index, row and column given are in bound*/
bool inbound(int index, int row, int col);

/*Targets the block with the highest probability value*/
int prob_attack(char* statement, struct float_board *prob_board, struct board *opp_board);

/*Targets the blocks around a ship*/
int target_attack(char* statement, int target, struct board *opp_board);

/*Checks if a ship was sunk by opponent*/
bool check_ship_sunk(struct board *own_board, char* statement);

/*Checks if all ships were sunk by opponent*/
bool check_concede(struct board *board);

/*Marks an element of the ship array of the board as sunk*/
void mark_sunk_ship(struct board *board, int ship_size);

#endif /*_COMMANDS_H*/
