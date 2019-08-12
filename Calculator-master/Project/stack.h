#ifndef _STACK_
#define _STACK_
#include <stdio.h>
#include "number.h"
typedef struct node{
	number num;
	struct node *next;
}node;
typedef node *stack;
void sinit(stack *s);
void push(stack *s, number num);
number pop(stack *s);
int isempty(stack *s);
int isfull(stack *s);

#endif
