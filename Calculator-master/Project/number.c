#include "number.h"
#include <stdio.h>
#include <stdlib.h>


int init(number **num, char *charact, char *dec) {
    if(*num == NULL)
    	*num = malloc(sizeof(number));	
    if(*num == NULL)
    	return -1;
    digit *new_digit = NULL;
    digit *prev_digit = NULL;
    int i=0;
    if(charact[0] == '-'){
    	(*num)->sign = 1;
    	i = 1;
    }
    // creating list of characteristic.
    while(charact[i] != '\0') {
        new_digit = malloc(sizeof(digit));
        if(new_digit == NULL) return -1;

        new_digit->num = charact[i] - '0';
        new_digit->next = prev_digit;
        prev_digit = new_digit;
        i++;
    }
    (*num)->characteristic = new_digit;

    new_digit  = NULL;
    prev_digit = NULL;

    i = 0;
    // creating list of decimal.
    while(dec[i] != '\0') {
        new_digit = malloc(sizeof(digit));
        if(new_digit == NULL) return -1;

        new_digit->num = dec[i] - '0';
        new_digit->next = prev_digit;
        prev_digit = new_digit;
        i++;
    }
    (*num)->decimal = new_digit;
    return 0;
}

number add(number *left, number *right) {
    number result;
    initNumber(&result);
   result.sign = -1;
    int flag = 0;
	
	if(compare(left, right) == 1 || compare(left, right) == 0){
		if(left->sign == 0 && right->sign == 1){ // + -
			right->sign = 0;
			result = subtract(left, right);
			result.sign = 0;
			return result;
		}else if(left->sign == 1 && right->sign == 0){// - +
			left->sign = 0;
			result = subtract(right, left);
			result.sign = 1;
			return result;
		}else if(left->sign == 1 && right->sign == 1){// + -
			result.sign = 1;
		}else if(left->sign == 0 && right->sign == 0){
			result.sign = 0;
		}	
	}else{
		if(left->sign == 0 && right->sign == 1){
			right->sign = 0;
			result = subtract(left, right);
			result.sign = 1;
			return result;
		}else if(left->sign == 1 && right->sign == 0){
			left->sign = 0;
			result = subtract(right, left);
			result.sign = 0;
			return result;
		}else if(left->sign == 1 && right->sign == 1){
			result.sign = 1;
		}else if(left->sign == 0 && right->sign == 0){
			result.sign = 0;
		}
	}
	
	
    int carry = add_digits(left, right, DEC, 0);
    if(carry == -1) 
    	return result;

    carry = add_digits(left, right, CHARACT, carry);
    if(carry == -1)
    	return result;

    if(carry) {
    	append_to_tail(left, CHARACT, 1);
    	result = *left;
        return result;
    }
    result = *left;

    return result;
}

int add_digits(number *left, number *right, int part, int carry) {
    fix_length(left, right, part);

    digit *cur_left  = (part == CHARACT) ? left->characteristic : left->decimal;
    digit *cur_right = (part == CHARACT) ? right->characteristic : right->decimal;
    digit *prev_left = NULL;

    while(cur_left != NULL || cur_right != NULL || carry) {
        if(cur_left != NULL && cur_right != NULL) {
            carry = add_with_carry(cur_left, cur_right, carry);

        } else if(cur_right != NULL) {
            cur_left = malloc(sizeof(digit));
            if(cur_left == NULL) {
                return -1;
            }

            carry = add_with_carry(cur_left, cur_right, carry);
            cur_left->next = NULL;
            prev_left->next = cur_left;

        } else if(cur_left != NULL && carry) {
            carry = add_with_carry(cur_left, NULL, carry);
        } else if(cur_left == NULL && cur_right == NULL && carry) { //For fractional parts of equal length.
            return carry;
        }

        prev_left = cur_left;
        if(cur_left  != NULL)
        	cur_left  = cur_left->next;
        if(cur_right != NULL)
        	cur_right = cur_right->next;
    }

    return 0;
}

int add_with_carry(digit *int1, digit *int2, int carry) {
    int num = (int2 != NULL) ? int2->num : 0;

    int sum = int1->num + num + carry;
    carry = (sum >= 10) ? 1 : 0;
   	int1->num = sum % 10;

    return carry;
}

number mult(number *left, number *right) {
    number *shift_num = shift_full_right(right);
    number *i = NULL;
    number *one = NULL;
	number result;
    init(&i, "1", "0");
    init(&one, "1", "0");
    number *orig_left;
  	number *res = NULL;
    result.characteristic = NULL;
    result.decimal = NULL;
    int flag = 0;
    digit *tmp = right->characteristic;
   	//Handling multiplication with zero.
    while(tmp != NULL){
    	if(tmp->num != 0)
    		break;
    	else
    		tmp = tmp->next;
    }
    
    if(!tmp){
    	tmp = right->decimal;
    	while(tmp){
    		if(tmp->num != 0)
    		 break;
    		else
    			tmp = tmp->next;
    	}
    }
    
    if(tmp == NULL){
		init(&res, "0", "0");
		return *res;
	}
    
    //Checking for numbers are +ve or -ve.
    if(left->sign == 1 && right->sign == 1){
    	result.sign = 0;
    }
    else if(left->sign == 1 || right->sign == 1){
    	flag = 1;
    	result.sign = 1;
    }
    left->sign = 0;
    right->sign = 0;
    if(copy_number(left, &orig_left) == -1){
    	result.sign = -1;
    	return result;
	}
    while(compare(i, right) == -1) {
        if(add(left, orig_left).sign == -1){
		    result.sign = -1;
			return result;
        }
        add(i, one);
    }
    
    shift_left(left, shift_num);
    shift_left(right, shift_num);
    result = *left;
    if(flag == 1)
    	result.sign = 1;
    return result;
}

number power(number *base, number *pow) {
    number *i = NULL;
    number *one = NULL;
    number result;
    initNumber(&result);
    init(&i, "1", "0");
    init(&one, "1", "0");
	result = *base;
    number *orig_base;
    if(copy_number(base, &orig_base) == -1){
    	result.sign = -1;	
    	return result;
    }
    while(compare(i, pow) == -1) {
        if(mult(base, orig_base).sign == -1){
        	result.sign = -1;	
	    	return result;
	    }
        add(i, one);
    }
	
    return *base;
}
//Takes two numbers and check whether they are equal or not.
int compare(number *num1, number *num2) {
    int result = compare_digits(num1, num2, CHARACT);

    if (result == 0) {
        result = compare_digits(num1, num2, DEC);
    }

    return result;
}

int compare_digits(number *num1, number *num2, int part) {
    if(num1 == num2) //If the same number is compared.
    	return 0;

    fix_length(num1, num2, part);

    digit *cur_num1  = (part == CHARACT) ? reverse_list(num1->characteristic) : reverse_list(num1->decimal);
    digit *cur_num2 = (part == CHARACT) ? reverse_list(num2->characteristic) : reverse_list(num2->decimal);

    if(part == CHARACT) {
        num1->characteristic  = cur_num1;
        num2->characteristic = cur_num2;
    } else {
        num1->decimal  = cur_num1;
        num2->decimal = cur_num2;
    }

    int ret = 2;
    while(cur_num1 != NULL && cur_num2 != NULL) {
        if(cur_num1->num > cur_num2->num) {
            ret = 1;
            break;
        } else if(cur_num1->num < cur_num2->num) {
            ret = -1;
            break;
        }

        if(cur_num1  != NULL)
        	cur_num1  = cur_num1->next;
        if(cur_num2 != NULL)
        	cur_num2 = cur_num2->next;
    }

    if(ret == 2) {
        if(cur_num1 != NULL) {
            ret = 1;
        } else if(cur_num2 != NULL) {
            ret =  -1;
        } else {
            ret = 0;
        }
    }

    cur_num1  = ((part == CHARACT) ? reverse_list(num1->characteristic) : reverse_list(num1->decimal));
    cur_num2 = ((part == CHARACT) ? reverse_list(num2->characteristic) : reverse_list(num2->decimal));

    if(part == CHARACT) {
        num1->characteristic  = cur_num1;
        num2->characteristic = cur_num2;
    } else {
        num1->decimal  = cur_num1;
        num2->decimal = cur_num2;
    }

    return ret;
}

int fix_length(number *left, number *right, int part) {
    digit *cur_left  = (part == CHARACT) ? left->characteristic : left->decimal;
    digit *cur_right = (part == CHARACT) ? right->characteristic : right->decimal;

    while(cur_left != NULL || cur_right != NULL) {
        if(cur_left == NULL) {
            ((part == CHARACT) ? append_to_tail(left, part, 0) : append_to_head(left, part, 0));
        } else if(cur_right == NULL) {
            ((part == CHARACT) ? append_to_tail(right, part, 0) : append_to_head(right, part, 0));
        }

        if(cur_left  != NULL) cur_left  = cur_left->next;
        if(cur_right != NULL) cur_right = cur_right->next;
    }

    return 0;
}

int append_to_head(number *num, int part, int data) {
    digit *new = malloc(sizeof(digit));
    if(new == NULL)
    	return -1;
    if(part == CHARACT) {
        new->next = num->characteristic;
        new->num  = data;
        num->characteristic = new;
    } else if(part == DEC) {
        new->next = num->decimal;
        new->num  = data;
        num->decimal = new;
    }

    return 0;
}

int append_to_tail(number *num, int part, int data) {
	int flag = 0;
	digit *list;
    digit *new = malloc(sizeof(digit));
    new->num = data;
    new->next = NULL;
    if(new == NULL)
    	return -1;
    if(part == CHARACT){
		list = num->characteristic;
		flag = 1;
	}else{
		list = num->decimal;
	}
	
	if(flag == 1){
		if(num->characteristic == NULL){
			num->characteristic = new;
			return 0;
		}
	}else{
		if(num->decimal == NULL){
			num->decimal = new;
			
			return 0;
		}
	}
    digit *cur = list;

    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new;
   
    return 0;
}

number* shift_full_right(number *num) {
    digit *tmp = num->characteristic;
    num->characteristic = num->decimal;

    num->decimal = NULL;

    number *shift_num = NULL;
    number *one = NULL;
    init(&shift_num, "0", "0");
    init(&one, "1", "0");

    digit *cur = num->characteristic;
    while(cur->next != NULL) {
        cur = cur->next;
        add(shift_num, one);
    }
    add(shift_num, one);
    cur->next = tmp;

    return shift_num;
}

int shift_left(number *num, number *shift_num) {
    digit *cur = num->characteristic;
    digit *prev = NULL;

    number *i = NULL;
    number *one = NULL;
    init(&i, "0", "0");
    init(&one, "1", "0");

    while(compare(i, shift_num) == -1) {
        prev = cur;
        cur = cur->next;
        add(i, one);
    }
    prev->next = NULL;

    digit *dec_tail = num->decimal;
    if(dec_tail == NULL) {
        num->decimal = num->characteristic;
    } else {
        while(dec_tail->next != NULL) {
            dec_tail = dec_tail->next;
        }
        dec_tail->next = num->characteristic;
    }

    num->characteristic = cur;

    return 0;
}

int copy_number(number *num, number **copy) {
    *copy = malloc(sizeof(number));
    if(copy == NULL) return -1;
    (*copy)->characteristic = NULL;
    (*copy)->decimal = NULL;

    if(copy_list(num->characteristic, &((*copy)->characteristic)) == -1) return -1;
    if(copy_list(num->decimal, &((*copy)->decimal)) == -1) return -1;

    return 0;
}

int copy_list(digit *head, digit **copy_head) {
    digit *cur = head;
    digit *prev = NULL;
    digit *new;
    int first = 1;

    while(cur != NULL) {
        new = malloc(sizeof(digit));
        if(new == NULL) return -1;
        if(first) {
            (*copy_head) = new;
            first = 0;
        } else {
            prev->next = new;
        }

        new->next = NULL;
        new->num = cur->num;

        prev = new;
        cur = cur->next;
    }

    return 0;
}

void print_number(number *num) {
    num->characteristic = reverse_list(num->characteristic);
    num->decimal = reverse_list(num->decimal);
    digit *cur = num->characteristic;
    digit *tmp = num->characteristic;
    //sign handling  
    while(tmp != NULL){
    	if(tmp->num != 0)
    		break;
    	else
    		tmp = tmp->next;
    }
    if(!tmp){
    	tmp = num->decimal;
    	while(tmp){
    		if(tmp->num != 0)
    		 break;
    		else
    			tmp = tmp->next;
    	}
    }
    if(!tmp)
    	num->sign = 0;
    if(num->sign == 1)
    	printf("-");
    while(cur != NULL) {
        printf("%d", cur->num);
        cur = cur->next;
    }

    printf(".");

    cur = num->decimal;
    while(cur != NULL) {
        printf("%d", cur->num);
        cur = cur->next;
    }
    printf("\n");
    num->characteristic = reverse_list(num->characteristic);
    num->decimal = reverse_list(num->decimal);
}

digit* reverse_list(digit *head) {
    digit *prev = NULL;
    digit *cur  = head;
    digit *tmp  = NULL;

    while(cur != NULL) {
        tmp = cur->next;
        cur->next = prev;
        prev = cur;
        cur = tmp;
    }

    return prev;
}

//Subtracts decimal parts.
int subDec(number *num1, number *num2, number *result){
	digit *p;
	digit *q;
	int sub;
	p = num1->decimal;
	q = num2->decimal;
	result->decimal = NULL;
	while(p && q){
		if(p->num >= q->num){
			append_to_tail(result, DEC, (p->num - q->num));
		}else{
			if(p->num == '0'){
				sub = 10 - q->num;
				append_to_tail(result, DEC, sub);
				if (p->next){
					if(p->next->num != '0')
						p->next->num--;
					else 
						p->next->num = '9';
				}	
			}else{			
				sub = (p->num - '0') + 10 - (q->num - '0');
				append_to_tail(result, DEC, sub);
				
				if (p->next != NULL){
					if (p->next->num != '0')
						p->next->num--;   //borrow
					else 
						p->next->num = '9';
				}else {
					return 1;
					
				}
			}
		}
		p = p->next;
		q = q->next;
	}
	return 0;
}



//Finds length of the given part.
int lengthOfPart(number *num, int part){
	int len = 0;
	digit *ptr;	
	ptr = (part == DEC) ? num->decimal : num->characteristic;
	while(ptr){
		ptr = ptr->next;
		len++;
	}
	
	return len;	
}

//Pads zero.

void padZero(number *num, int noOfZeros){
	while(noOfZeros--){
		append_to_head(num , DEC, 0);
	}	
}

int subChar(number *num1, number *num2, number *result, int borrow){
	digit *p;
	digit *q;
	int sub;
	result->characteristic = NULL;
	p = num1->characteristic;
	q = num2->characteristic;
	if(borrow == 1)
		p->num--;
	while(p && q){
		if(p->num >= q->num){
			append_to_tail(result, CHARACT, (p->num - q->num));
		}else{
			if(p->num == '0'){
				sub = 10 - q->num;
				append_to_tail(result, CHARACT, sub);
				if (p->next){
					if(p->next->num != '0')
						p->next->num--;
					else 
						p->next->num = '9';
				}	
			}else{			
				sub = (p->num - '0') + 10 - (q->num - '0');
				append_to_tail(result, CHARACT, sub);
				if (p->next)
					if (p->next->num != '0')
						p->next->num--;   //borrow
					else 
						p->next->num = '9';
			}
		}
		p = p->next;
		q = q->next;
	}
	
	return 0;
}





number subtract(number *num1, number *num2) {
	number *tmp, *ptr, result, **n;
	int borrow;
	ptr = &result;
	int decLen1;
	int decLen2;
	int swap = 0;
	digit *x;
	result.sign = 0;
	
	if(compare(num1, num2) == 0 && num1->sign == 0 && num2->sign == 0){
		result.sign = 0;
		init(&ptr, "0", "0");
		return result;
	}
	else {
		if(compare_digits(num1, num2, CHARACT) == 1){   //num1 > num2
			result.sign = 0;
		}else if(compare_digits(num1, num2, CHARACT) == 0){
			if(compare_digits(num1, num2, DEC) == -1){
				tmp = num1;
				num1 = num2;
				num2 = tmp;
				swap = 1;	
			}
		}else if(compare_digits(num1, num2, CHARACT) == -1){
			tmp = num1;
			num1 = num2;
			num2 = tmp;
			swap = 1;
		}
	} //Ensured that the first digit is greater.    eg. 12.1267 > 1.31231
	
	if(num1->sign == 0 && num2->sign == 1){ // + -
		num1->sign = 0;
		num2->sign = 0;
		result = add(num1, num2);
		if(swap)		
			result.sign = 1;
		else
			result.sign = 0;
		return result;
	}else if(num1->sign == 1 && num2->sign == 0){ // - +
		num1->sign = 0;
		num2->sign = 0;
		result = add(num1, num2);
		if(swap)
			result.sign = 0;
		else
			result.sign = 1;
		
		return result;
	}else if(num1->sign == 1 && num2->sign == 1){ // - -
		if(swap)
			result.sign = 0;
		else	
			result.sign = 1;
	}else if(num1->sign == 0 && num1->sign == 0){
		if(swap)
			result.sign = 1;
		else	
			result.sign = 0;
	}	
	
	if(swap && num1->sign == 1 && num2->sign == 1)
		result.sign = 0;
	
	//Find length of decimal for each number.
	decLen1 = lengthOfPart(num1, DEC);
	decLen2 = lengthOfPart(num2, DEC);
	
	//pad zeros to number of smaller length.
	if(decLen1 > decLen2){
		padZero(num2, decLen1 - decLen2);	
	}else{
		padZero(num1, decLen2 - decLen1);
	}
	//Now decimal parts are of equal length.

	borrow = subDec(num1, num2, &result); // Subtracting decimals of numbers
	
	subChar(num1, num2, &result, borrow);
	removeZeros(&result);
	return result;
}




void destroyNumber(number *num){
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


void removeZeros(number *num){
	digit *p, *q;
	if(lengthOfPart(num, CHARACT) == 1)
		return;
	p = num->characteristic;
	q = num->characteristic;
	while(p->next)
		p = p->next;
	while(q->next != p){
		q = q->next;
	}
	if(p->num != 0){
		return;
	}
	else{
		q->next = NULL;
		free(p);
		removeZeros(num);
	}
}


void initNumber(number *num){
	num->characteristic = NULL;
	num->decimal = NULL;
	num->sign = 0;
}

number leftshift(number num1, number num2){
	number n,  p;
	number b, *ptr;
	ptr = &b;
	init(&ptr, "2", "0");
	p = power(&b, &num2);
	n = mult(&num1, &p);
	return n;
}


number mod(number *p, number *q){
	number result;
	initNumber(&result);
	result = subtract(p, q);
	while(compare(&result, q) != -1){
		result = subtract(&result, q);
	}
	return result;
}

