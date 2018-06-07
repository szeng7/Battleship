/*Simon Zeng */
/* Main program to run */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "stack.h"

int main(int argc, char *argv[])
{	

	int row, target, target_row, target_col_num, ship_size;
	char col, target_col;
	char statement[10] = {'\0'};
	char result_of_attack[10] = {'\0'};

	struct board own_board;
	struct board *own_board_pntr = &own_board;
	own_board.concede = 0;
	init_board(own_board_pntr);
	set_ships(own_board_pntr);

	struct board opp_board;
	struct board *opp_board_pntr = &opp_board;
	opp_board.concede = 0;
	init_board(opp_board_pntr);

	struct float_board prob_board;
	struct float_board *prob_board_pntr = &prob_board;
	prob_board.concede = 0;
	init_float_board(prob_board_pntr);

	struct Stack coord_bank;
	struct Stack *coord = &coord_bank;
	Stack_Init(coord);

	if (argc == 1) { //attacking second/player 2
		
		scanf("%s", statement);

		if (strcmp(statement, "CONCEDE") == 0) {
			return EXIT_SUCCESS;
		}

		if (scanf(" %c %d", &col, &row) != 2) {
			fprintf(stderr, "error: statement given is invalid\n");
			return EXIT_SUCCESS;
		}

		if (decide_result(statement, row, col, own_board_pntr) == 0) {
			fprintf(stderr, "error: statement given is invalid\n");
			return EXIT_SUCCESS;
		}

		printf("%s\n", statement);
		memset(statement, '\0', strlen(statement));
		fflush(stdout);
	}
	
	update_prob_board(opp_board_pntr, prob_board_pntr);
	target = prob_attack(statement, prob_board_pntr, opp_board_pntr);

	target_col_num = target % 10;
	target_row = ((target - target_col_num) / 10) + 1;
	target_col = 'A' + target_col_num;

	printf("%s %c %d\n", statement, target_col, target_row);
	memset(statement, '\0', strlen(statement));
	fflush(stdout);

	while (scanf("%s", result_of_attack) == 1) {	

		if (strcmp(result_of_attack, "CONCEDE") == 0) {
			return EXIT_SUCCESS;
		}

		if (strcmp(result_of_attack, "SUNK") == 0) {
			scanf("%d", &ship_size);
		}

		scanf("%s %c %d", statement, &col, &row);		
		if (decide_result(statement, row, col, own_board_pntr) == 0) {
			fprintf(stderr, "error: statement given is invalid\n");
			return EXIT_SUCCESS;
		}
		if (strcmp(statement, "CONCEDE") == 0) {
			printf("%s\n", statement);
			return EXIT_FAILURE;
		}

		printf("%s\n", statement);
		memset(statement, '\0', strlen(statement));
		fflush(stdout);
	
		int prev = update_opp_board(result_of_attack, opp_board_pntr, target_row,
				target_col, ship_size);
		
		if (prev > 0) {
			Stack_Push(coord, prev);
			Stack_Push(coord, prev);
			Stack_Push(coord, prev);
			Stack_Push(coord, prev);
		} else if (prev == 0) {
			while (Stack_Empty(coord) != true) {
				Stack_Pop(coord);
			}
		}
	
		memset(result_of_attack, '\0', strlen(result_of_attack));

		update_prob_board(opp_board_pntr, prob_board_pntr);

		if (Stack_Empty(coord) != true) {
			target = Stack_Pop(coord);	
			target = target_attack(statement, target, opp_board_pntr);
		} else {
			target = prob_attack(statement, prob_board_pntr, opp_board_pntr);
		}
	
		target_col_num = target % 10;
		target_row = ((target - target_col_num) / 10) + 1;
		target_col = 'A' + target_col_num;

		if (opp_board.r[target_row-1][target_col_num] != 0 || target == 0) {
			while (Stack_Empty(coord) != true) {
				Stack_Pop(coord);
			}
			target = prob_attack(statement, prob_board_pntr, opp_board_pntr);
			
			target_col_num = target % 10;
			target_row = ((target - target_col_num) / 10) + 1;
			target_col = 'A' + target_col_num;
		}	

		printf("%s %c %d\n", statement, target_col, target_row);
		memset(statement, '\0', strlen(statement));
		fflush(stdout);
	}
}
