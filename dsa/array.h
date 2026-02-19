#ifndef ARRAY_H
#define ARRAY_H

// #include "../server/models/users.h"
// #include <stdlib.h>
typedef struct {
	void** base;
	int len;
	int cap;
} array;



// #define DECLARE_VAR(TYPE, NAME) TYPE NAME

/* defines struct for array of provided type */
#define DEFINE_ARR_TYPE(TypeName, x_Type) \
    typedef struct TypeName { \
        x_Type* base; \
	int cap; \
	int len; \
    } TypeName; \

/* defines header for  creating array of type */
#define DEFINE_ARR_CREATE_H(Typename,x_Type) \
	Typename create_##Typename(void); \

/* defines header for expandind array of type */
#define DEFINE_ARR_EXPAND_H(Typename,x_Type) \
	void expand_##Typename(Typename* arr); \

/* defines header for appending array of type */
#define DEFINE_ARR_APPEND_H(Typename,x_Type) \
	void append_##Typename(Typename* arr, x_Type* elt); \


/* defines function to create an array of the provided type */
#define DEFINE_ARR_CREATE(Typename, x_Type) \
	Typename create_##Typename(void) { \
		Typename arr; \
		arr.cap = 10; \
		arr.len = 0; \
		arr.base = calloc(10, sizeof(x_Type)); \
		return arr; \
};

/* defines function to expand an array of the provided type */
#define DEFINE_ARR_EXPAND(Typename, x_Type) \
	void expand_##Typename(Typename* arr) { \
		int new_cap = (arr->cap) * 2; \
		arr->base = realloc(arr->base, new_cap * sizeof(x_Type)); \
		arr->cap = new_cap; \
	};

#define DEFINE_TYPE_TO_VOID_FUNC(x_Type)\
	typedef void (void_from_##x_Type)(x_Type)\

#define DEFINE_ARR_FOR_EACH(Typename, x_Type)\
	void for_each_##Typename(Typename* src, void_from_##x_Type fn) {\
		for (int i = 0; i < src->len; i++) {\
			(fn)(src->base[i]);\ 
		}\
	}


/* defines header for for_each_##Typename function */
#define DEFINE_ARR_FOR_EACH_H(Typename, x_Type)\
	void for_each_##Typename(Typename* src, void_from_##x_Type fn);\

/* defines function to append an array of the provided type */
#define DEFINE_ARR_APPEND(Typename, x_Type)  \
	void append_##Typename(Typename* arr, x_Type* elt) { \
		if (arr->len  >=  arr->cap) { \
			expand_##Typename(arr); \
		} \
		arr->base[arr->len] = *elt; \
		arr->len++; \
	};

/* defines bool_from_##x_Type as a function that returns a bool from an x_Type */
#define DEFINE_TYPE_TO_BOOL_FUNC(x_Type)\
	typedef bool (bool_from_##x_Type)(x_Type)\


/* defines a function that filters the provided array using the provided bool_from_##x_Type function */
#define DEFINE_ARR_FILTER(Typename, x_Type)\
	Typename filter_##Typename(Typename* arr, bool_from_##x_Type fn) {\
		Typename ret = create_##Typename();\
		for (int i = 0; i < arr->len; i++) {\
			if ((fn)(arr->base[i])) { \
				append_##Typename(&ret, &(arr->base[i]));\
			}\
		}\
		return ret;\
	}

/* defines header for array filter function */
#define DEFINE_ARR_FILTER_H(Typename, x_Type)\
	Typename filter_##Typename(Typename* arr, bool_from_##x_Type fn);\


/** 
 * defines a function that set copies that value at index i to the ret pointer 
 * returns 1 if index is out of bounds
 **/
#define DEFINE_ARR_GET_AT_IDX(Typename, x_Type)\
	int get_at_idx_##Typename(Typename* arr, int i, x_Type* ret) {\
		if (i >= arr->len) {\
			return 1;\
		}\
		*ret = arr->base[i];\
		return 0;\
	}

/* defines a header for the get_at_idx_##Typename function */
#define DEFINE_ARR_GET_AT_IDX_H(Typename, x_Type)\
	int get_at_idx_##Typename(Typename* arr, int i, x_Type* ret);\

/* defines a function that gets that last element of the arr */
#define DEFINE_ARR_GET_LAST(Typename, x_Type)\
	x_Type get_last_##Typename(Typename* arr) {\
		return arr->base[arr->len - 1];\
	}

/* defines a header for get_last_##Typename */
#define DEFINE_ARR_GET_LAST_H(Typename, x_Type)\
	x_Type get_last_##Typename(Typename* arr);\

/* 
 defines a header for get_last_##Typename 
 #define DEFINE_ARR_PRINT(Typename, x_Type)\
 	void print_##Typename(Typename* arr) {\
 		print_##x_Type(arr);\
 	}

 #define DEFINE_ARR_PRINT_H(Typename, x_Type)\
 	void print_##Typename(Typename* arr);\
*/

#define DEFINE_ARR_FUNCTIONS(Typename, x_Type)\
	DEFINE_ARR_CREATE(Typename, x_Type); \
	DEFINE_ARR_EXPAND(Typename, x_Type); \
	DEFINE_ARR_APPEND(Typename, x_Type); \
	DEFINE_ARR_FOR_EACH(Typename, x_Type);\
	DEFINE_ARR_FILTER(Typename, x_Type);\
	DEFINE_ARR_GET_LAST(Typename, x_Type);\
	DEFINE_ARR_GET_AT_IDX(Typename, x_Type);\



#define DEFINE_ARR_H(Typename, x_Type) \
	DEFINE_ARR_TYPE(Typename, x_Type); \
	DEFINE_ARR_CREATE_H(Typename, x_Type); \
	DEFINE_ARR_EXPAND_H(Typename, x_Type); \
	DEFINE_ARR_APPEND_H(Typename, x_Type); \
	DEFINE_TYPE_TO_VOID_FUNC(x_Type);\
	DEFINE_TYPE_TO_BOOL_FUNC(x_Type);\
	DEFINE_ARR_FOR_EACH_H(Typename, x_Type);\
	DEFINE_ARR_FILTER_H(Typename, x_Type);\
	DEFINE_ARR_GET_LAST_H(Typename, x_Type);\
	DEFINE_ARR_GET_AT_IDX_H(Typename, x_Type);\


#endif
