#include<stdio.h>
#include "number.h"
#include<string.h>
#include<stdlib.h>
#define MAX 128
void destroynumber(number *num){
	digit *p, *q;
	p = num->characteristic;
	while(p){
		q = p;
		p = p->next;
		free(q);
	}
	p = num->decimal;
	while(p){
		q = p;
		p = p->next;
		free(q);
	}
}

int main() {
	number a[MAX];
	number b;
	number *aptr, *bptr;
	digit *ptr;
	bptr = &b;
	int i = 0;
	char *man;
	char *dec;
	char m[MAX];
	char d[MAX];
	int n = 2;
	initNumber(&a[0]);
	initNumber(&a[1]);
	initNumber(&b);
	while(n--){
		aptr = &a[i];
		scanf("%s%s", m, d);
		//man = (char *)malloc(strlen(m) + 1);
		//dec = (char *)malloc(strlen(d) + 1);
	//For a
	//strcpy(man, m);
	//strcpy(dec, d);
		init(&aptr, m, d);
		print_number(&a[i++]);
	//free(man);
	//free(dec);
	}
	//b = subtract(&a[0], &a[1]);
	//print_number(&b);
	/*
	b = subtract(&a[0], &a[1]);
	ptr = b.characteristic;
	while(ptr){
		printf("%c", ptr->num);
		ptr = ptr->next;
	}
	ptr = b.decimal;	
	printf(".");
	while(ptr){
		printf("%c", ptr->num);
		ptr = ptr->next;
	}printf("\n");
	*/
	//b = add(&a[0], &a[1]);	
	//print_number(&a[0]);
	b = mod(&a[0], &a[1]);
	//print_number(&a[0]);
	print_number(&b);
	destroyNumber(&a[0]);
	destroyNumber(&a[1]);
	destroyNumber(&b);

	return 0;	
}
