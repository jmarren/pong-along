
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stdlib.h>
#include "../app.h"
#include "shared.h"


uv_connect_t *req;
Uint32 read_event_type;


void on_write_end(uv_write_t *write_req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }

    free(write_req);
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

    buf.base = malloc(sizeof(char) * strlen(message));
    buf.len = strlen(message);
    buf.base = message;
    uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    int buf_count = 1;

    uv_write(write_req, req->handle, &buf, buf_count, on_write_end);
}



void on_connect(uv_connect_t *new_req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
	
    req = new_req;
    uv_read_start(req->handle, alloc_buffer, read_data);
}



void tcp_init(App* app) {

   read_event_type = app->read_event_type;


    // allocate the socket
    uv_tcp_t* tcp_sock = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	
    // initialize tcp handle
    int err = uv_tcp_init(app->loop, tcp_sock);
    if (err) {
        fprintf(stderr, "TCP init error: %s\n", uv_strerror(err));
    }

    // allocate the connection struct
    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    // build the socket destination address struct
    struct sockaddr_in tcp_dest;
    uv_ip4_addr("0.0.0.0", 7000, &tcp_dest);

    // call connect using the on_connect callback
    // no need to connect udp
    uv_tcp_connect(connect, tcp_sock, (const struct sockaddr*)&tcp_dest, on_connect);
}
