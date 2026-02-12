#ifndef NET_H
#define NET_H



// #include "uv.h"
#include <SDL3/SDL_events.h>
// #include <stdlib.h>

extern SDL_Event net_read_evt;
void init_net(void);
// void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
// void on_write_end(uv_write_t *write_req, int status);
void write_on_connection(char *message);
// char* parse_message(char* msg);
// void read_data(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
// void on_connect(uv_connect_t *new_req, int status);
void start_uv_thread(void* args);



#endif

