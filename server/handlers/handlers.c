
#include "uv.h"
#include "../../shared/parse.h"
#include "../../shared/buffer.h"
#include "../../shared/callback.h"
#include <stdlib.h>
#include <string.h>
#include "../server.h"


// void get_other_users(uv_stream_t* client, users_list* other_users) {
//
// 	other_users->count = 0;
// 	other_users->users = malloc(sizeof(user) * 10);
//
// 	for (int i = 0; i < active_users.count; i++) {
// 		user user = active_users.users[i];
// 		if (user.stream != client) {
// 			other_users->users[other_users->count] = user;
// 			other_users->count++;
// 		}
// 	}
// }
//


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

	users_arr_append(&(server->active_users), &user);

}

void handle_players_query(server_t* server, uv_stream_t* client, message* msg) { 

	// store a ref to active users
	// users_arr* active_users = &(server->active_users); 

	// allocate the response message
	char* res = calloc(100, sizeof(char));

	// // copy the type to res
	// strcpy(res, "players:");

	char* other_usernames = users_arr_other_users_usernames(client, &(server->active_users));

	sprintf(res, "players:%s\r\n", other_usernames);

	printf("res = %s\n", res);
	

	
	// // cat each username onto the res with a comma
	// for (int i = 0; i < active_users->len; i++) {
	// 	if (active_users->users[i].stream != client) {
	// 		char* username = active_users->users[i].username;
	// 		if (strlen(username) + strlen(res) > 100) { 
	// 			break;
	// 		}
	// 		strcat(res, username);
	// 		strcat(res, ",");
	// 	}
	// }
	
	// append delimiter
	// strcat(res, "\r\n");

	// initialize the write buffer with the size of the message that was read (to echo back)
	uv_buf_t wrbuf = uv_buf_init(res, 100);
		
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));


	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, cb_free_write);
}

