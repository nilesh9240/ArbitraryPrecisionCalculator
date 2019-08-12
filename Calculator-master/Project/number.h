#ifndef _INTEGER_
#define _INTEGER_
#include <stdio.h>
#include <stdlib.h>

#define CHARACT 1
#define DEC 2

typedef struct digit {// A simple list
    char num;
    struct digit *next;
}digit;

typedef struct number {	//pointer to two lists....
    struct digit *characteristic;	//1]Characteristic 2] Decimal
    struct digit *decimal;
    int sign;	//+ve = 0, -ve = 1					
}number;


// Takes two strings as characteristic and decimal and coverts into a number.
int init(number **num, char *charact, char *dec);

//Adds two numbers and stores the sum in first number.
number add(number *left, number *right);

//Adds digits of the specified part and returns the carry.
int add_digits(number *left, number *right, int part, int carry);

//Adds the single digit and returns the carry.
int add_with_carry(digit *left, digit *right, int carry);

//Multiplies two numbers
number mult(number *left, number *right);

//Finds power of two numbers
number power(number *base, number *pow);

//Returns 0 when numbers are equal,  returns -1 when not equal and returns 1 when num1 > num2
int compare(number *num1, number *num2);

//Compares the specified part of the number.
int compare_digits(number *num1, number *num2, int part);

//Prints the number
void print_number(number *num);

//Subtracts decimal part
int subDec(number *num1, number *num2, number *result);

//Gives length of parts
int lengthOfPart(number *num, int part);

//adjuses zeros
void padZero(number *num, int noOfZeros);

//Subtracts Characteristic part
int subChar(number *num1, number *num2, number *result, int borrow);

//Subtracts two numbers
number subtract(number *num1, number *num2);

//Destroys number
void destroyNumber(number *num);

//removes extra zeros
void removeZeros(number *num);

//initialises number
void initNumber(number *num);

//leftshift operation
number leftshift(number num1, number num2);

//Mod operation
number mod(number *p, number *q);

int fix_length(number *left, number *right, int part);
int append_to_head(number *num, int part, int data);
int append_to_tail(number *num, int part, int data);
number* shift_full_right(number *num);
int shift_left(number *num, number *shift_num);
int copy_number(number *num, number **copy);
int copy_list(digit *head, digit **copy_head);
digit* reverse_list(digit *head);

#endif
