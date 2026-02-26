// #include "../server.h"
//
//
// void create_invite(server_t* server, user_t* invitor, user_t* invitee) {
// 	invite invite = {
// 		.invitor = invitor,
// 		.invitee = invitee,
// 	};
//
// 	server->invites.base[server->invites.len] = invite;
// 	server->invites.len++;
// }
//
// static invite* get_last(server_t* server) {
// 	invite* curr = server->invites.base;
// 	invite* prev;
//
// 	while (curr != NULL) {
// 		prev = curr;
// 		curr = curr->next;
// 	}
//
//
// 	return prev;
// }
//
// static void append(server_t* server, invite* new_invite) {
// 	invite* last = get_last(server);
// 	last->next = new_invite;
// }
//
// // static void remove(server_t* server, 
