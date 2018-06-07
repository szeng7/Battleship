/* Simon Zeng szeng7@jhu.edu, Bryan Ki nki1@jhu.edu */
/* Main program to run (that uses the commands) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

int main(int argc, char *argv[]) 
{
	int our_board[100] = {0};
	int opp_board[100] = {0};
	int row, f, g, atk_colNum, atk_row, sunk_spot;
	char col, atk_col;
	char command[5];
	char statement[10] = {'\0'};	
	char attack[10] = {'\0'};	
	int index = 10;
	
	init_board(our_board);
	
	if (argc > 1) { /* attacking first/player1 */
		
		printf("FIRE A 2\n"); /* block we always want to start with */
		fflush( stdout );	
		while ((f = scanf("%s", attack)) == 1) {	
			if (strcmp(attack, "SUNK") == 0) {
				if (scanf("%d\n%s %c %d", 
					&sunk_spot, command, &col, &row) != 4) {
					fprintf(stderr, "error: scanf failed\n");
					return EXIT_FAILURE;
				}
			} else if (strcmp(attack, "CONCEDE") == 0) {
				return EXIT_SUCCESS;
			} else {
				if (scanf("%s %c %d", command, &col, &row) != 3) {
					fprintf(stderr, "error: scanf failed\n");
					return EXIT_FAILURE;
				}
			}
		
			if (command == NULL) {
				return EXIT_FAILURE;
			}
			if ((g = decide_result(command, col, row, 
						our_board, statement)) == 1) {
				printf("%s\n", statement);
				memset(statement, '\0', strlen(statement));
			} else if (g == 2) {
				printf("%s\n", statement);
				memset(statement, '\0', strlen(statement));
			} else if (g == 3) {
				printf("%s\n", statement);
				return EXIT_FAILURE;
			} else { //if g = 0
				return EXIT_FAILURE;
			}
			index = calc_index(opp_board, index, attack);
			if (index < 0) {
				return EXIT_SUCCESS;
			}

			atk_colNum = index % 10;
			atk_row = ((index - atk_colNum) / 10) + 1;
			atk_col = 'A' + atk_colNum;
			calc_attack(attack);
			printf("%s %c %d\n", attack, atk_col, atk_row);
			fflush (stdout );
		}	
	} else { /* receiving first/player 2, without -f flag */
		if (scanf("%s %c %d", command, &col, &row) != 3) {
			fprintf(stderr, "error: scanf failed\n");
			return EXIT_FAILURE;
		}

		if (command == NULL) {
			return EXIT_FAILURE;
		}
		if ((g = decide_result(command, col, row, 
					our_board, statement)) == 1) {
			printf("%s\n", statement);
			memset(statement, '\0', strlen(statement));
		}
		
		printf("FIRE A 2\n"); 
		fflush ( stdout );

		while ((f = scanf("%s", attack)) == 1) {	
			if (strcmp(attack, "SUNK") == 0) {
				if (scanf("%d\n%s %c %d", &sunk_spot, 
						command, &col, &row) != 4) {
					fprintf(stderr, "error: scanf failed\n");
					return EXIT_FAILURE;
				}
			} else if (strcmp(attack, "CONCEDE") == 0) {
				return EXIT_SUCCESS;
			} else {
				if (scanf("%s %c %d", command, &col, &row) != 3) {
					fprintf(stderr, "error: scanf failed\n");
					return EXIT_FAILURE;
				}
			}
		
			if (command == NULL) {
				return EXIT_FAILURE;
			}
			if ((g = decide_result(command, col, row, 
						our_board, statement)) == 1) {
				printf("%s\n", statement);
				memset(statement, '\0', strlen(statement));
			} else if (g == 2) {
				printf("%s\n", statement);
				memset(statement, '\0', strlen(statement));
			} else if (g == 3) {
				printf("%s\n", statement);
				return EXIT_FAILURE;
			}else { //if g = 0
				return EXIT_FAILURE;
			}	

			index = calc_index(opp_board, index, attack);
		
			if (index < 0) {
				/* opponent conceded */
				return EXIT_SUCCESS;
			}

			atk_colNum = index % 10;
			atk_row = ((index - atk_colNum) / 10) + 1;
			atk_col = 'A' + atk_colNum;
			calc_attack(attack);
			printf("%s %c %d\n", attack, atk_col, atk_row);
			fflush (stdout );
		}		
	}
	if (f != 3) {
		fprintf(stderr, "error: input is of wrong format\n");
		return EXIT_FAILURE;
	}
	
	argv++; /* just to bypass compiler warning */
	return EXIT_SUCCESS;
} 
