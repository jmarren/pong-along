
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../app.h"

uv_connect_t *req;
Uint32 read_event_type;
uv_udp_t* udp_sock;
struct sockaddr_in udp_dest;


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

// typedef void (*uv_udp_send_cb)(uv_udp_send_t* req, int status);
void udp_send_cb(uv_udp_send_t* req, int status) { 
	printf("udp send cb fired\n");
}

void write_udp(char* message) {

    uv_buf_t buf;

    buf.base = malloc(sizeof(*message));
    buf.len = strlen(message);
    buf.base = message;
    // uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    int buf_count = 1;

	
    printf("writing to udp_sock\n");

    uv_udp_send_t* req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));



    uv_udp_send(req, udp_sock, &buf, buf_count, (const struct sockaddr*)&udp_dest, udp_send_cb);

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

    int err;

    // create default loop
    uv_loop_t* loop = uv_default_loop();

    udp_sock = (uv_udp_t*)malloc(sizeof(uv_udp_t));
	
    // allocate the socket
    uv_tcp_t* tcp_sock = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	
    // initialize tcp
    err = uv_tcp_init(loop, tcp_sock);
    if (err) {
        fprintf(stderr, "TCP init error: %s\n", uv_strerror(err));
    }
    err = uv_udp_init(loop, udp_sock);
    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }


    // allocate the connection struct
    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    // build the socket destination address struct
    struct sockaddr_in tcp_dest;
    uv_ip4_addr("0.0.0.0", 7000, &tcp_dest);

    uv_ip4_addr("0.0.0.0", 11000, &udp_dest);

    // err = uv_udp_bind(udp_sock, (const struct sockaddr*)&udp_dest, UV_UDP_REUSEADDR);
    // if (err) {
    //     fprintf(stderr, "UDP bind error: %s\n", uv_strerror(err));
    // }
    // err = uv_udp_connect(udp_sock, (const struct sockaddr*)&udp_dest);
    // if (err) {
    //     fprintf(stderr, "UDP connect error: %s\n", uv_strerror(err));
    // }

    // call connect using the on_connect callback
    uv_tcp_connect(connect, tcp_sock, (const struct sockaddr*)&tcp_dest, on_connect);

    // run the loop
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
}



