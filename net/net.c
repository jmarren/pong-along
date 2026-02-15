
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../app.h"

uv_connect_t *req;
Uint32 read_event_type;


/* PRIVATE */
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_write_end(uv_write_t *write_req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
	
    free(write_req);
}


char* trim_message(char* msg) {
	
	for (int i = 0; i < (int)strlen(msg); i++) {
		if (msg[i] == '\r') {
			msg[i] = '\0';
		}
	}
	return msg;
}


void read_data(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
   if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*)stream, NULL);
        }
    } else if (nread > 0) {
	// initialize a user event
	SDL_UserEvent event;
	
	// set event type 
	event.type = read_event_type;
	
	// set data1 to the buffer base pointer
	event.data1 = (void*)buf->base;
	
	// push event
    	SDL_PushEvent((SDL_Event*)&event);

    } else {
	printf("read 0\n");
    }
}


void net_write(char *message) {

    uv_buf_t buf;

    buf.base = malloc(sizeof(*message));
    buf.len = strlen(message);
    buf.base = message;
    uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    int buf_count = 1;

    uv_write(write_req, req->handle, &buf, buf_count, on_write_end);
}

/* PUBLIC */
void net_init(App* app) {
	// set the read event type in global var
	read_event_type = app->read_event_type;
}







void on_connect(uv_connect_t *new_req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
	
    req = new_req;
    uv_read_start(req->handle, alloc_buffer, read_data);
}




void net_start(void* args) {

    // create default loop
    uv_loop_t* loop = uv_default_loop();
	
    // allocate the socket
    uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	
    // initialize tcp
    uv_tcp_init(loop, socket);

    // uv_poll_init_socket(loop, (uv_poll_t *)&poll, *(uv_os_sock_t*)(socket));

    // allocate the connection struct
    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    // build the socket destination address struct
    struct sockaddr_in dest;
    uv_ip4_addr("0.0.0.0", 7000, &dest);

    // call connect using the on_connect callback
    uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);

    // run the loop
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
}



