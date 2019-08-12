#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "stack.h"
#include "cstack.h" 
#include "number.h"
#include <ctype.h>
#define MAX_LEN 128


int readline(char *arr, int n) {
	char ch;
	int i = 0;
	while((ch = getchar()) != '\n' && i < n)
		arr[i++] = ch;
	arr[i] = '\0';
	return i;
}




#define OPERATOR	100
#define	OPERAND		200
#define	END		300
#define	ERROR		400
typedef struct token{
	int type;
	union data {
		number num;
		char op;
	}data;
}token;



enum states {START, DIG, OP, STOP, ERR, SPC};

token getnext(char *str, int *restart) {
	static int currstate = START;
	int nextstate;
	static int i = 0;
	token t;
	int sum = 0;
	char currchar, currop;
	int index = 0;
	char characteristic[MAX_LEN];
	char mantissa[MAX_LEN];
	int k = 0;
	int j = 0;
	int flag = 0;
	number *aptr;
	aptr = &(t.data.num);
	mantissa[0] = '0';
	characteristic[0] = '0';
	for(index = 1; index < MAX_LEN; index++){
		mantissa[index] = '\0';
		characteristic[index] = '\0';
	}
	initNumber(aptr);
	if(*restart == 1) {
		i = 0;
		currstate = START;
		*restart = 0;
	}
	while(1) {
		currchar = str[i++];
		if(isdigit(str[i]) && currchar == '-'){
			currchar = str[i];
			j = 1;
			characteristic[0] = '-';
			i++;	
		}
		if(currchar == '<')
			i++;
		switch(currstate) {
			case START:
				switch(currchar) {
					case '.':
						flag = 1;
						k = 0; 
						break;
					case '0': case '1': case '2':
					case '3': case '4': case '5':
					case '7': case '8': case '9':
					case '6':
						nextstate = DIG;
						if(k == 1){
							mantissa[k++] = currchar;
						}else{
							characteristic[j++] = currchar;
						}
						
						break;
					case '+': case '-': case '*':
					case '/': case '%': case '(':
					case ')': case '<': case '^':
						nextstate = OP;
						currop = currchar;	
						break;
					case ' ': case '\t':
						nextstate = SPC;
						break;
					case '\0':
						nextstate = STOP;
						break;
					default:
						break;
				}
				break;
			case DIG:
				switch(currchar) {
					case '.':
						flag = 1;
						k = 0; 
						break;
					case '0': case '1': case '2':
					case '3': case '4': case '5':
					case '7': case '8': case '9':
					case '6':
						if(flag == 1){
							mantissa[k++] = currchar;
						}else{
							characteristic[j++] = currchar;
						}
						nextstate = DIG;						
						break;
					case '+': case '-': case '*':
					case '/': case '%': case '(':
					case ')': case '<': case '^':
						nextstate = OP;
						t.type = OPERAND;
						init(&aptr, characteristic, mantissa);
						mantissa[0] = '0';
						characteristic[0] = '0';
						for(index = 1; index < MAX_LEN; index++){
							mantissa[index] = '\0';
							characteristic[index] = '\0';
						}
						k = 0;
						j = 0;
						currstate = nextstate;
						flag = 0;
						return t;	
						break;
					case ' ': case '\t':
						nextstate = SPC;
						t.type = OPERAND;
						init(&aptr, characteristic, mantissa);
						mantissa[0] = '0';
						characteristic[0] = '0';
						for(index = 1; index < MAX_LEN; index++){
							mantissa[index] = '\0';
							characteristic[index] = '\0';
						}
						k = 0;
						j = 0;
						flag = 0;
						currstate = nextstate;
						return t;	
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERAND;
						init(&aptr, characteristic, mantissa);
						mantissa[0] = '0';
						characteristic[0] = '0';
						for(index = 1; index < MAX_LEN; index++){
							mantissa[index] = '\0';
							characteristic[index] = '\0';
						}
						k = 0;
						j = 0;
						flag = 0;
						currstate = nextstate;
						return t;	
						break;
					default:
						nextstate = ERR;
						t.type = OPERAND;
						init(&aptr, characteristic, mantissa);
						mantissa[0] = '0';
						characteristic[0] = '0';
						for(index = 1; index < MAX_LEN; index++){
							mantissa[index] = '\0';
							characteristic[index] = '\0';
						}
						k = 0;
						j = 0;
						flag = 0;
						currstate = nextstate;
						return t;	
						break;
				}

				break;
			case OP:
				switch(currchar) {
					case '0': case '1': case '2':
					case '3': case '4': case '5':
					case '7': case '8': case '9':
					case '6':
						nextstate = DIG;
						characteristic[j++] = currchar;
						t.type = OPERATOR;
						t.data.op = currop;
						currop = currchar;
						currstate = nextstate;
						return t;
						break;
					case '+': case '-': case '*':
					case '/': case '%': case '(':
					case ')': case '<': case '^':
						nextstate = OP;
						t.type = OPERATOR;
						t.data.op = currop;
						currop = currchar;
						currstate = nextstate;
						return t;
						break;
					case ' ': case '\t':
						nextstate = SPC;
						t.type = OPERATOR;
						t.data.op = currop;
						currstate = nextstate;
						return t;
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERATOR;
						t.data.op = currop;
						currstate = nextstate;
						return t;
						break;
					default:
						nextstate = ERR;
						t.type = OPERATOR;
						t.data.op = currop;
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case SPC:
				switch(currchar) {
					case '0': case '1': case '2':
					case '3': case '4': case '5':
					case '7': case '8': case '9':
					case '6':
						nextstate = DIG;
						characteristic[j++] = currchar;
						break;
					case '+': case '-': case '*':
					case '/': case '%': case '(':
					case ')': case '<': case '^':
						nextstate = OP;
						currop = currchar;
						break;
					case ' ': case '\t':
						nextstate = SPC;
						break;
					case '\0':
						nextstate = STOP;
						break;
					default:
						nextstate = ERR;
						break;
				}

				break;
			case STOP:
				t.type = END;
				return t;
				break;
			case ERR:
				t.type = ERROR;
				return t;
				break;
		}
		currstate = nextstate;
	

	}
}

/*
 * Infix evaluation main logic,get the token and evaluate the expression
 */
number infixeval(char *infix){
	int restart = 1;
	char str, opr, x;
	stack s, s1;
	cstack cs, cs1;
	sinit(&s);
	sinit(&s1);
	cinit(&cs);
	cinit(&cs1);
	token t;
	number result, tmp;
	number n1, n2;
	initNumber(&result);
	initNumber(&tmp);
	initNumber(&n1);
	initNumber(&n2);
	while(1) {
		t = getnext(infix, &restart);
		if(t.type == OPERAND){
			push(&s, t.data.num);
		}
		else if(t.type == END){
			while(!cisempty(&cs)){
				if(ctop(&cs) == '*' || ctop(&cs) == '%' || ctop(&cs) == '^'){
					n1 = pop(&s);
					n2 = pop(&s);
					switch(cpop(&cs)){
						case '*':
							tmp = mult(&n1, &n2);
							push(&s1, tmp);
							break;
						case '%':
							tmp = mod(&n2, &n1);
							push(&s1, tmp);
							break;
						case '^':
							tmp = power(&n2, &n1);
							push(&s1, tmp);
							break;
						default:
							return tmp;
					}
				}else
					cpush(&cs1, cpop(&cs));
				
			}
			while(!isempty(&s)){
				push(&s1, pop(&s));
			}
			while(!cisempty(&cs1)){
				x = cpop(&cs1);
				n1 = pop(&s1);
				n2 = pop(&s1);
				switch(x){
		            case '+':
		            	    result = add(&n1, &n2);
		                    push(&s1, result);
		                    break;
		            case '-':
		                    result = subtract(&n1, &n2);
		                    push(&s1, result);
		                    break;/*
		            case '/':
		                    push(&s, (n1 / n2));
		                    break;*/
		            case '*':
		            		result = mult(&n1, &n2);
		                    push(&s1, result);
		                    break;
		            case '<':
		            		result = leftshift(n1, n2);
		            		push(&s1, result);
		            		break;
		            case '%':
		                    result = mod(&n1, &n2);
		                    push(&s1, result);
		                    break;
		            case '^':
							tmp = power(&n2, &n1);
							push(&s1, tmp);
							break;
		            default:
		                    return result; 
           		}
	
			}
			return pop(&s1);
		}
		else if(t.type == ERROR)
			return result;
		else if(t.type == OPERATOR){
			if(cisempty(&cs))
				cpush(&cs, t.data.op);
			else if(t.data.op == '(')
				cpush(&cs, t.data.op);
			else if(t.data.op == ')'){
				while((str = cpop(&cs)) != '('){
					n2 = pop(&s);
					n1 = pop(&s);
					switch(str){
						case '+':
							result = add(&n1, &n2);
                            push(&s, result);
							break;
						case '-':
							result = subtract(&n1, &n2);
	                    	push(&s, result);
							break;/*
						case '/':
							push(&s, (n1 / n2));
							break;*/
						case '*':
							result = mult(&n1, &n2);					
							push(&s, result);
							break;
						case '<':
            				result = leftshift(n1, n2);
            				push(&s, result);
            				break;
						case '%':
							result = mod(&n1, &n2);
							push(&s, result);
							break;
						case '^':
							tmp = power(&n2, &n1);
							push(&s1, tmp);
							break;
						default:
							return result;
					}
					
				}
			
			}
			else{
				opr = ctop(&cs);
				if(precedence(t.data.op) >= precedence(opr)){
					cpush(&cs, t.data.op);
				}
				else if(precedence(t.data.op) < precedence(opr)){
					n2 = pop(&s);
        			n1 = pop(&s);
					opr = cpop(&cs);
					cpush(&cs, t.data.op);
	                switch(opr){
	                        case '+':
	                                result = add(&n1, &n2);
	                                push(&s, result);
	                                break;
	                        case '-':
	                                result = subtract(&n1, &n2	);
									push(&s, result);
	                                break;
	                       /* case '/':
	                                push(&s, (n1 / n2));
	                                break;
	                     */   case '*':
                     				result = mult(&n1, &n2);
	                                push(&s, result);
	                                break;
	                       	  case '<':
									result = leftshift(n1, n2);
									push(&s, result);
									break;
	                       	  case '%':
	                       	  		result = mod(&n1, &n2);
	                                push(&s, result);
	                                break;
	                          case '^':
									tmp = power(&n2, &n1);
									push(&s1, tmp);
									break;
	                        default:
	                                return result;
	                	
	                }
				}
			}
		
		}
	}
	destroyNumber(&n1);
	destroyNumber(&n2);
	destroyNumber(&result);	
	return result;
}
char ctop(cstack *s) {
	char temp = cpop(s);
	cpush(s, temp);
	return temp;
}
int precedence(char op) {
	if(op == '^')
		return 90;
	if(op == '%')
		return 70;
	if(op == '*' || op == '/')
		return 50;
	if(op == '+' || op == '-')
		return 30;
	if(op == '<')
		return 20;
	return 0;
}


/*main function*/
int main(){
	char line[128];
	int x;
	number y;
	while((x = readline(line, 128))) {
		y = infixeval(line);
		print_number(&y);
		printf("\n");
	}
	return 0;
}
