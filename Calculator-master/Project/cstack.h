#include<stdio.h>
typedef struct cnode{
        char ch;
        struct cnode *cnext;
}cnode;
typedef cnode *cstack;
void cinit(cstack *s);
void cpush(cstack *s, char c);
char cpop(cstack *s);
int cisempty(cstack *s);
int cisfull(cstack *s);
char ctop(cstack *s);
int precedence(char c);

