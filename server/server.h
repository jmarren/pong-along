#ifndef SERVER_H
#define SERVER_H
 

#include "models/users.h"
#include "uv.h"


typedef struct {
	users_arr active_users;
	uv_loop_t* loop;
	uv_tcp_t tcp_server;
	uv_udp_t udp_server;
} server_t;


#endif
