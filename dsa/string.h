#ifndef STRING_H
#define STRING_H
#include "array.h"
#include <stdbool.h>

DEFINE_ARR_H(string, char);
DEFINE_ARR_H(string_arr, string);


void print_string_arr(string_arr* arr);
void set_string(string* s, char* new_str);
string join_string_arr(string_arr* arr, char* delim);
void printS(string s);
void println(string s);

#endif
