
#include "uv.h"
#include "../../shared/parse.h"
#include "../../shared/buffer.h"
#include "../../shared/callback.h"
#include <stdlib.h>
#include <string.h>
#include "../server.h"

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
	char* username = calloc(1, strlen(msg->content));
	strcpy(username, msg->content);

	// create a new user
	user_t user = {
		username,
		client,
	};

	append_user_arr(&(server->active_users), &user);

}

void handle_players_query(server_t* server, uv_stream_t* client, message* msg) { 


	user_arr other_users = filter_not_client(&(server->active_users), client);

	string_arr usernames = get_usernames(&other_users);
	
	string comma_joined = join_string_arr(&usernames, ", ");


	uv_buf_t wrbuf;

	buffer_encode_assign("players", comma_joined.base, &wrbuf);
		
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));


	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, cb_free_write);
}

