#ifndef _STACK_H
#define _STACK_H

#define STACK_MAX 10
#include <stdbool.h>
#include "commands.h"

struct Stack
{
	int data[STACK_MAX];
	int size;
};

void Stack_Init(struct Stack *S);

void Stack_Push(struct Stack *S, int d);

int Stack_Pop(struct Stack *S);

bool Stack_Empty(struct Stack *S);

#endif /*_STACK_H */
