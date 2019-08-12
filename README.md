# Arbitrary Precision Calculator

Name : Nilesh Ankush Kavhale

MIS ID : 111603064

College : College of Engineering, Pune

Title of Project : Basic calculator

A basic calculator project which can evaluate arithmetic expressions containing large numbers(which cannot be stored in primitive data type).

Description :
	1)I have used two singly linked null terminated lists to store characteristic part and decimal part separately.
	2)Each node of these lists is been typedefed as digit, digit is a struct containing next pointer and a character.
	3)The two lists and an integer is combined in a struct and pointer to this struct is been typedefed to a user defined type named "number"
	4)Integer in the struct represents sign of the number, 1 for -ve and 0 for +ve.
	5)Each single digit of the number is stored as a character.
	
operations implemented:
	+ , - , <<, power, mod, unary minus.
  
Feature:
  Expression containing multiiple operators is evaluated according to priority of operators
