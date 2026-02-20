#ifndef SERVER_H
#define SERVER_H

#include "uv.h"
#include "../../../shared/macro.h"


typedef struct {
	char username[MAX_USERNAME_CHARS];
	uv_stream_t* stream;
} user_t;

typedef struct {
	int len;
	user_t users[MAX_ACTIVE_USERS];
} active_users;


typedef struct {
	active_users active_users;
	uv_loop_t* loop;
	uv_tcp_t tcp_server;
	uv_udp_t udp_server;
} server_t;


#endif
