#ifndef SERVER_H
#define SERVER_H
 

#include "uv.h"

typedef struct {
	char* username;
	uv_stream_t* stream;
} user;



typedef struct {
	user* users;
	int count;
} users_list;


typedef struct {
	users_list active_users;
	uv_loop_t* loop;
	uv_tcp_t tcp_server;
	uv_udp_t udp_server;
} server_t;


#endif
