#include "stack.h"
#include "commands.h"

#include <stdbool.h>
#include <stdio.h>

void Stack_Init(struct Stack *S)
{
    S->size = 0;
}

void Stack_Push(struct Stack *S, int d)
{
	 S->data[S->size++] = d;
}

int Stack_Pop(struct Stack *S)
{
	int target = S->data[S->size-1];
	S->size--;
    return target;
}

bool Stack_Empty(struct Stack *S)
{
	return S->size == 0;
}

