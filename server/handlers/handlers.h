#ifndef HANDLERS_H
#define HANDLERS_H

#include "uv.h"
#include "../server.h"
#include "../../shared/parse.h"

void handle_username(server_t* server, uv_stream_t* client, message* msg);
void handle_players_query(server_t* server, uv_stream_t* client, message* msg);
void handle_selected_opponent(server_t* server, uv_stream_t* client, message* msg);

#endif
