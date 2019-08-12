#include "cstack.h"
#include <stdlib.h>
void cinit(cstack *s){
        *s = NULL;
}

void cpush(cstack *s, char c){
        cnode *temp, *last = *s;
        temp  = (cnode*)malloc(sizeof(cnode));
        temp->ch = c;
        temp->cnext = NULL;
        if(*s == NULL){
                *s = temp;
        }
        else{
                while(last->cnext != NULL)
                        last = last->cnext;
                last->cnext = temp;
        }
}

char cpop(cstack *s){
        int flag = 0;
	char c;
        cnode *temp = NULL;
        cnode *last = *s;
        while(last->cnext != NULL){
                temp = last;
                last = last->cnext;
                flag = 1;
        }
        c = last->ch;
        if(flag == 0){
                *s = NULL;
        }
        else
                temp->cnext = NULL;
        free(last);
        return c;
}

int cisempty(cstack *s){
        return *s == NULL;
}

int cisfull(cstack *s){
        cnode *temp = (cnode*)malloc(sizeof(cnode));
        if(temp == NULL){
                return 1;
        }
        else{
                free(temp);
                return 0;
        }
}
