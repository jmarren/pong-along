#include "users.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../../dsa/array.h"

#define SUCCESS 0
#define FAIL 1


void print_user_t(user_t* user) { 
	printf("%s\n", user->username);
}

char* stringify_user_t(user_t* user) { 
	return user->username;
}

DEFINE_ARR_FUNCTIONS(user_arr, user_t);


void user_print_username(user_t user) {
	printf("username = %s\n", user.username);
}

void print_usernames(user_arr* arr) {
	for_each_user_arr(arr, &user_print_username);
}





// #define DECLARE_VAR(TYPE, NAME) TYPE NAME

// #define ARRAY_CREATE(x) (
//
// )

// users_arr* users_arr_create(void) {
// 	array ret = array_create();
// 	return (users_arr*)&ret;
// }


// initialize a new users_arr
void users_arr_init(users_arr* users_arr, int initial_cap, int max_cap) { 
	users_arr->len = 0;
	users_arr->users = calloc(initial_cap, sizeof(user_t));
	users_arr->capacity = initial_cap;
	users_arr->max_capacity = max_cap;
}

// expand array capacity
int users_arr_expand(users_arr* users_arr) {

	// check if at or above max cap
	if (users_arr->capacity >= users_arr->max_capacity) {
		return FAIL;
	}

	int new_capacity;

	// if doubling exceeds max, use max
	if (users_arr->capacity * 2 > users_arr->max_capacity) {
		new_capacity = users_arr->max_capacity;
	} else {
 	// otherwise double
		new_capacity = users_arr->capacity * 2;
	}

	// reallocate to new cap
	users_arr->users = realloc(users_arr->users, new_capacity * sizeof(user_t));
	// set new cap
	users_arr->capacity = new_capacity;
	
	return SUCCESS;
}

// append a user to the array
int users_arr_append(users_arr* users_arr, user_t* new_user) {
	
	// try expanding if at capacity
	if (users_arr->len >= users_arr->capacity) {
		// return failure if at max cap
		if (users_arr_expand(users_arr) == FAIL) return FAIL;
	}

	// copy new user to the end of the arr
	users_arr->users[users_arr->len] = *new_user;

	// incr length
	users_arr->len++;

	return 0;
}

// get last member
user_t* users_arr_get_last(users_arr* users_arr) {
	return &(users_arr->users[users_arr->len - 1]);
}


// get user at provided index
// returns 1 if out of bounds
int users_arr_get_at(users_arr* users_arr, int i, user_t* user) {
	// return 1 if out of bounds
	if (i >= users_arr->len) return FAIL;
		
	// copy the user to the provided use pointer
	*user = users_arr->users[i];

	return SUCCESS;
}



void users_arr_print_usernames(users_arr* src) {
	
	for (int i = 0; i < src->len; i++) {
		printf("%s ", src->users[i].username);
	}
	printf("\n");

}


void users_arr_for_each(users_arr* arr, user_to_void fn) {
	for (int i = 0; i < arr->len; i++) {
		(fn)(&(arr->users[i]));
	}
}


// create a user_arr
// for each user
//   do func
//  return user_arr
//

void* users_arr_map(users_arr* arr, user_to_void_ptr fn, void* ret) {
	
	ret = malloc(arr->len * sizeof(void*));
		
	for (int i = 0; i < arr->len; i++) {
		
	}

	
	// users_arr_for_each(arr, fn);
}


void users_arr_filter_in_place(users_arr* src, user_to_bool func) {

	int i = 0;

	// for (; i < src->len - 1; i++) {
	// 	bool result = func(&(src->users[i]));
	// 	if (!result) {
	// 		src->users[i] = src->users[i + 1];
	// 	}
	// }
}

users_arr users_arr_filter(users_arr* src, user_to_bool fn) {
		
	users_arr ret;
	
	users_arr_init(&ret, 10, 100);

	// users_arr_for_each(src, )

	
	for (int i = 0; i < src->len; i++) {
		user_t user = src->users[i];
		bool result = (*fn)(&user);
		if (result) {
			users_arr_append(&ret, &user);
		}
	}

	return ret;
}


void users_arr_get_other_users(uv_stream_t* client, users_arr* src, users_arr* dest) {
	for (int i = 0; i < src->len; i++) {
		if (src->users[i].stream != client) {
			users_arr_append(dest, &(src->users[i]));
		}
	}
	printf("done\n");
}


char* users_arr_stringify_usernames(users_arr* src) {
	char* ret = calloc(100, sizeof(char));

	for (int i = 0; i < src->len; i++) {
		printf("ret = %s\n", ret);
		strcat(ret, src->users[i].username);
		if (i < src->len - 1) {
			strcat(ret, ",");
		}
	}	
	
	return ret;
}

char* users_arr_other_users_usernames(uv_stream_t* client, users_arr* src) {
	users_arr dest;
	users_arr_get_other_users(client, src, &dest);
	char* res =  users_arr_stringify_usernames(&dest);
	return res;

}


