
#include "uv.h"
#include "../../shared/parse.h"
#include <stdlib.h>
#include <string.h>
#include "../server.h"


void write_callback(uv_write_t *req, int status) {
    if (status) {
	printf("write error\n");
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
	
    free(req);
}

void handle_selected_opponent(uv_stream_t* client, message* msg) {

	char* res = malloc(strlen(msg->content) + strlen("match:\r\n"));
		
	sprintf(res, "match:%s\r\n", msg->content);

	// initialize the write buffer with the size of the message that was read (to echo back)
	uv_buf_t wrbuf = uv_buf_init(res, strlen(res) + 1);
		
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, write_callback);
}


void handle_username(server_t* server, uv_stream_t* client, message* msg) {
	char* username = calloc(1, strlen(msg->content));
	strcpy(username, msg->content);

	// create a new user
	user user = {
		username,
		client,
	};

	// add the user to active_users
	server->active_users.users[server->active_users.count] = user;
	server->active_users.count++;
}

void handle_players_query(server_t* server, uv_stream_t* client, message* msg) { 

	users_list* active_users = &(server->active_users); 

	// allocate the response message
	char* res = calloc(100, sizeof(char));

	// copy the type to res
	strncpy(res, "players:", strlen("players:"));
	
	// cat each username onto the res with a comma
	for (int i = 0; i < active_users->count; i++) {
		if (active_users->users[i].stream != client) {
			char* username = active_users->users[i].username;
			printf("username = %s\n", username);
			strcat(res, username);
			strcat(res, ",");
		}
	}
	
	// append delimiter
	strcat(res, "\r\n");

	// initialize the write buffer with the size of the message that was read (to echo back)
	uv_buf_t wrbuf = uv_buf_init(res, 100);
		
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));


	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, write_callback);
}

