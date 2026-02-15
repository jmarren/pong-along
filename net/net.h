#ifndef NET_H
#define NET_H

#include <SDL3/SDL_events.h>
#include "../app.h"

// the event that is fired when data is read from the connection
extern SDL_Event net_read_evt;

// initializes the net lib 
//  -> register read event with SDL
void net_init(App* app);

// writes a message to the tcp connection
void net_write(char *message);

// starts the uv loop
void net_start(void* args);


// char* parse_message(char* msg);
// void read_data(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
// void on_connect(uv_connect_t *new_req, int status);
// void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
// void on_write_end(uv_write_t *write_req, int status);


#endif

