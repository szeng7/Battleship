/*Bryan Ki nki1@jhu.edu, Simon Zeng szeng7@jhu.edu */
/*This is a basic test driver for the battle program*/

#include <assert.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "commands.h"

int main(void)
{
	int our_board[100];
	char statement[10];
	int index = 0;
	int cntr = 10;

	init_board(our_board);
	assert(!decide_result("CHEAT", 'A', 0, our_board, NULL));
	assert(!decide_result("fire", 'A', 0, our_board, NULL));
	assert(!decide_result("FIRE", 'K', 0, our_board, NULL));
	assert(!decide_result("FIRE", 'A', 11, our_board, NULL));
	calc_attack(statement);
	
	/* These refused to be recognized by the gcov when used in indirectly
	 * within commands.c, so they had to be called with an external file
	 */	
	
	assert(strcmp(statement, "FIRE") == 0);		
	index = index_increment(index);
	assert(check_inbound(index) == true);
	assert(check_concede(cntr, statement) == 3);
}
