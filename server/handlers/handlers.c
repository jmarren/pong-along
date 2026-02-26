
#include "uv.h"
#include "../../shared/parse.h"
#include "../../shared/macro.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include "../server.h"
#include "../models/users.h"
#include "../models/invites.h"
#include "../tcp.h"



void handle_selected_opponent(server_t* server, uv_stream_t* client, message* msg) {
	printf("selected opponent username = %s\n", msg->content);
	
	user_t* invitor = get_user_from_stream(server, client);
	if (invitor == NULL) {
		printf("invitor not found\n");
	}

	user_t* invitee = get_user_with_username(server, msg->content);
	if (invitee == NULL) {
		printf("invitee not found\n");
	}

	if (invitee == NULL || invitor == NULL) {
		printf("match failed\n");
		write_tcp(client, "match", "failed");
		return;
	}

	// create_invite(server, invitor, invitee);
	
	// // send the invite to the invitee
	// write_tcp(invitee->stream, "invite", invitor->username);
	//
	// write_tcp(client, "match", msg->content);
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
	
	usernames[0] = '\0';

	get_others(server->active_users, client, usernames);

	write_tcp(client, "players", usernames);

}

