
#ifndef _READ_INPUT_H
#define _READ_INPUT_H

#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif //_MSC_VER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char* str);
int read_digit();
unsigned long long read_digit_ull();
int count_split(char* str, int count);

#endif //_READ_INPUT_H
