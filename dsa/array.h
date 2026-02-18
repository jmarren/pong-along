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
		} \
	}


#define DEFINE_ARR_FOR_EACH_H(Typename, x_Type)\
	void for_each_##Typename(Typename* src, void_from_##x_Type fn);\

/* defines function to append an array of the provided type */
#define DEFINE_ARR_APPEND(Typename, x_Type)  \
	void append_##Typename(Typename* arr, x_Type* elt) { \
		if (arr->len + 1 > arr->cap) { \
			expand_##Typename(arr); \
		} \
		arr->base[arr->len] = *elt; \
		arr->len++; \
	};


#define DEFINE_TYPE_TO_BOOL_FUNC(x_Type)\
	typedef bool (bool_from_##x_Type)(x_Type)\


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


#define DEFINE_ARR_FILTER_H(Typename, x_Type)\
	Typename filter_##Typename(Typename* arr, bool_from_##x_Type fn);\

#define DEFINE_ARR_FUNCTIONS(Typename, x_Type)\
	DEFINE_ARR_CREATE(Typename, x_Type); \
	DEFINE_ARR_EXPAND(Typename, x_Type); \
	DEFINE_ARR_APPEND(Typename, x_Type); \
	DEFINE_ARR_FOR_EACH(Typename, x_Type);\
	DEFINE_ARR_FILTER(Typename, x_Type);\



#define DEFINE_ARR_H(Typename, x_Type) \
	DEFINE_ARR_TYPE(Typename, x_Type); \
	DEFINE_ARR_CREATE_H(Typename, x_Type); \
	DEFINE_ARR_EXPAND_H(Typename, x_Type); \
	DEFINE_ARR_APPEND_H(Typename, x_Type); \
	DEFINE_TYPE_TO_VOID_FUNC(x_Type);\
	DEFINE_TYPE_TO_BOOL_FUNC(x_Type);\
	DEFINE_ARR_FOR_EACH_H(Typename, x_Type);\
	DEFINE_ARR_FILTER_H(Typename, x_Type);\



// DEFINE_ARR_H(int_arr, int)
// DEFINE_ARR_H(user_arr, user_t)
//


//
//

// Define a macro that takes the desired type name (alias) and optionally the internal tag name


// array array_create(void) {
// 	array arr;
//
// 	arr.len = 0;
// 	arr.cap = 10;
// 	arr.base = calloc(10, sizeof(void*));
// 	return  arr;
// }

	

// #define DEFINE_ARR_APPEND(Typename, Type) \


// void array_expand(array* arr) {
//
// 	int new_cap = (arr->cap * 2) * sizeof(void*);
//
// 	arr->base = realloc(arr->base, new_cap);
// 	arr->cap = new_cap; 
//
// }
//
//
// void array_append(array* arr, void* elt) {
// 	if (arr->len + 1 > arr->cap) {
// 		array_expand(arr);
// 	}
//
// 	arr->base[arr->len] = elt;
// 	arr->len++;
// }
//

//
// DEFINE_ARR_TYPE(int_arr, int);
// DEFINE_ARR_CREATE(int_arr, int);
// DEFINE_ARR_EXPAND(int_arr, int);
// DEFINE_ARR_APPEND(int_arr, int);
//

// #define DEF_ADD(name) \
//     int add_##name(int x, int y) { \
//     return x + y; \
// }


// DEFINE_STRUCT_TYPE(my_struct, int);


// #define MY_MEMBER_TYPE int
//
// // Use the macro within a structure definition
// struct MyStruct {
//     MY_MEMBER_TYPE data;
//     float another_member;
// };

// 1. Define a list of structure members using an X-macro pattern
// #define MEMBER_LIST(X) \
//     X(int, id)         \
//     X(double, value)   \
//     X(char, status)
//
//
// MEMBER_LIST(hi);
//
// #define LIST_OF_VARIABLES \
//     X(value1) \
//     X(value2) \
//     X(value3)
//
// #define X(name) int name;
// LIST_OF_VARIABLES
// #undef X
//




void array_append(array* arr, void* elt);
array array_create(void);
#endif
