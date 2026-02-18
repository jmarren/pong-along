#include "array.h"
#include "string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


DEFINE_ARR_FUNCTIONS(string, char);
DEFINE_ARR_FUNCTIONS(string_arr, string);
 


void printS(string s) {
	printf("%s", s.base);
}

void println(string s) {
	printf("%s\n", s.base);
}

void print_string_arr(string_arr* arr) {
	for_each_string_arr(arr, &printS);
}

void set_string(string* s, char* new_str) {

	int new_len = strlen(new_str);
	
	while (new_len > s->cap) { 
		expand_string(s);
	}

	s->base = new_str;
	s->len = new_len;
}

void concat_string(string* s, char* new_string) {
	strcat(s->base, new_string);
	s->len = strlen(s->base);
}

// void concat(
//
string join_string_arr(string_arr* arr, char* delim) {
	string str = create_string();

	for (int i = 0; i < arr->len; i++) { 
		concat_string(&str, arr->base[i].base);
		if (i < arr->len - 1) {
			concat_string(&str, delim);
		}
	}
	return str;
}
