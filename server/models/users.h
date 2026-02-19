#ifndef USERS_H
#define USERS_H

#include "uv.h"
#include <stdbool.h>
#include "../../dsa/array.h"
#include "../../dsa/string.h"
typedef struct {
	char* username;
	uv_stream_t* stream;
} user_t;



typedef struct {
	user_t* users;
	int len;
	int capacity;
	int max_capacity;
} users_arr;


DEFINE_ARR_H(user_arr, user_t);

typedef bool (user_to_bool)(user_t* user);
typedef void (user_to_void)(user_t* user);
typedef void* (user_to_void_ptr)(user_t* user);
void users_arr_init(users_arr* users_arr, int initial_cap, int max_cap);
// int users_arr_expand(users_arr* users_arr);
user_t* users_arr_get_last(users_arr* users_arr);
int users_arr_append(users_arr* users_arr, user_t* new_user);
int users_arr_get_at(users_arr* users_arr, int i, user_t* user);
char* users_arr_other_users_usernames(uv_stream_t* client, users_arr* src);
char* users_arr_stringify_usernames(users_arr* src);
void users_arr_get_other_users(uv_stream_t* client, users_arr* src, users_arr* dest);
users_arr users_arr_filter(users_arr* src, user_to_bool fn);
void users_arr_print_usernames(users_arr* src);
void print_usernames(user_arr* arr);

user_arr filter_not_client(user_arr* users, uv_stream_t* client);

string_arr get_usernames(user_arr* arr);
#endif
