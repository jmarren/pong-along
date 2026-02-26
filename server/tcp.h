#ifndef SERVER_TCP_H
#define SERVER_TCP_H

#include "server.h"
void server_init_tcp(server_t* server);
void write_tcp(uv_stream_t* client, char* type, char* content);
#endif
