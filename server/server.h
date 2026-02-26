#ifndef SERVER_H
#define SERVER_H

#include "uv.h"
#include "../../../shared/macro.h"


	// uv_buf_t wrbuf;
	//
	// printf("selected opponent username = %s\n", msg->content);
	//
	//
	//
	// // if (set_opponent(server, client, msg->content) != 0) {
	// // 	buffer_encode_assign("match", "failed", &wrbuf);
	// // 	printf("failed to set opponent\n");
	// // }
	// //
	// // encode to protocol format and assign to write buffer
	// buffer_encode_assign("match", msg->content, &wrbuf);
	//
	// // allocate the write request to write back
	// uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
	//
	// // write the message back to the client
	// // with buf len of 1 and a write callback that 
	// // frees the write request
	// uv_write(req, client, &wrbuf, 1, cb_free_write);


// typedef void (write_tcp)(void* stream, char* type, char* content);

typedef struct {
	char username[MAX_USERNAME_CHARS];
	uv_stream_t* stream;
	void* opponent;
	// write_tcp* write_tcp;
} user_t;

// typedef void (render_handler)(void* app);
typedef struct {
	int len;
	user_t users[MAX_ACTIVE_USERS];
} active_users;




typedef struct {
	user_t user;
	float block_y;
} player;

struct invite {
	user_t* invitor;
	user_t* invitee;
	struct invite* next;
};

typedef struct invite invite;

typedef struct {
	int len;
	struct invite* base;
} invites;

typedef struct {
	float circle_pos;
	player player_one;
	player player_two;
} game;


typedef struct {
	int len;
	game* games;
} games;


typedef struct {
	uv_loop_t* loop;
	uv_tcp_t tcp_server;
	uv_udp_t udp_server;
	active_users active_users;
	invites invites;
	games games;
} server_t;


#endif
