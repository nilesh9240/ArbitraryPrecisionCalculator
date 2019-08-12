#include "stack.h"
#include <stdlib.h>
void sinit(stack *s){
	*s = NULL;
}
void push(stack *s, number num){
	node *temp, *last = *s;
	temp  = (node*)malloc(sizeof(node));
	temp->num = num;	
	temp->next = NULL;
	if(*s == NULL){
		*s = temp;
	}
	else{
		while(last->next != NULL)
			last = last->next;
		last->next = temp;
	}
}
number pop(stack *s){
	number num;
	int flag = 0;
	node *temp = NULL;
	node *last = *s;
	while(last->next != NULL){
		temp = last;
		last = last->next;
		flag = 1;
	}
	num = last->num;
	if(flag == 0){
		*s = NULL;
	}
	else
		temp->next = NULL;
	free(last);
	return num; 
}
int isempty(stack *s){
	return *s == NULL;
}
int isfull(stack *s){
	node *temp = (node*)malloc(sizeof(node));
	if(temp == NULL){
		return 1;
	}
	else{
		free(temp);
		return 0;
	}
}

