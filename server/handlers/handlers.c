
#include "uv.h"
#include "../../shared/parse.h"
#include "../../shared/buffer.h"
#include "../../shared/callback.h"
#include "../../shared/macro.h"
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "../server.h"
#include "../models/users.h"

void handle_selected_opponent(uv_stream_t* client, message* msg) {
	
	// initialize write buffer
	uv_buf_t wrbuf;

	// encode to protocol format and assign to write buffer
	buffer_encode_assign("match", msg->content, &wrbuf);
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, cb_free_write);
}


void handle_username(server_t* server, uv_stream_t* client, message* msg) {

	printf("got username =  %s\n", msg->content);

	// create a new user
	user_t user = {
		.stream = client,
	};

	// copy in the username
	strncpy(user.username, msg->content, MAX_USERNAME_CHARS);

	
	server->active_users.users[server->active_users.len] = user; 
	server->active_users.len++;


}

void handle_players_query(server_t* server, uv_stream_t* client, message* msg) { 
	int usernames_len = (MAX_ACTIVE_USERS * MAX_USERNAME_CHARS);
	int commas_len = MAX_ACTIVE_USERS;
	char usernames[usernames_len + commas_len]; 
	
	printf("players query\n");
	printf("msg->type = %s\n", msg->type);
	printf("msg->content = %s\n", msg->content);
	
	usernames[0] = '\0';

	get_others(server->active_users, client, usernames);
	
	printf("others = %s\n", usernames);

	uv_buf_t wrbuf;

	buffer_encode_assign("players", usernames, &wrbuf);
		
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	printf("sending players response = %s\n", wrbuf.base);

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, cb_free_write);
}

