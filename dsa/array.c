#include "array.h"
#include <stdlib.h>


// DEFINE_ARR_CREATE(int_arr, int);
// DEFINE_ARR_EXPAND(int_arr, int);
// DEFINE_ARR_APPEND(int_arr, int);
//

DEFINE_ARRAY_FULL(int_arr, int)

// DEFINE_ARR_CREATE(int_arr, int);
// DEFINE_ARR_EXPAND(int_arr, int);
// DEFINE_ARR_APPEND(int_arr, int);

array array_create(void) {
	array arr;

	arr.len = 0;
	arr.cap = 10;
	arr.base = calloc(10, sizeof(void*));
	return  arr;
}

void array_expand(array* arr) {
	
	int new_cap = (arr->cap * 2) * sizeof(void*);

	arr->base = realloc(arr->base, new_cap);
	arr->cap = new_cap; 

}


void array_append(array* arr, void* elt) {
	if (arr->len + 1 > arr->cap) {
		array_expand(arr);
	}

	arr->base[arr->len] = elt;
	arr->len++;
}





