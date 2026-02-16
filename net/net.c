
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../app.h"

uv_connect_t *req;
Uint32 read_event_type;
uv_udp_t* udp_sock;
uv_loop_t* loop;

uv_buf_t udp_write_buf;
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


    udp_write_buf.base = malloc(sizeof(char) * strlen(message));
    udp_write_buf.len = strlen(message);
    udp_write_buf.base = message;
    uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
    int buf_count = 1;

    uv_write(write_req, req->handle, &udp_write_buf, buf_count, on_write_end);
}

void udp_send_cb(uv_udp_send_t* req, int status) { 
	printf("udp send cb fired\n");
	printf("status = %d\n", status);
	free(req);
}



void write_udp(char* message) {
    uv_buf_t buf;
    buf.base = message;
    buf.len = strlen(message) + 1;
    int buf_count = 1;
    int err;

    // initialize udp handle
    err = uv_udp_init(loop, udp_sock);
    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

    // struct sockaddr_in udp_destination;
    uv_udp_send_t req;

    err = uv_udp_send(&req, udp_sock, &buf, buf_count, (const struct sockaddr*)&udp_dest, udp_send_cb);

    printf("uv_udp_send result = %d\n", err);

    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

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


void init_udp(void) {

    udp_sock = (uv_udp_t*)malloc(sizeof(uv_udp_t));


    // initialize udp_dest so we can send to it later
    int err = uv_ip4_addr("0.0.0.0", 8888, &udp_dest);

    if (err) {
        fprintf(stderr, "UDP bind error: %s\n", uv_strerror(err));
    }
}

void init_tcp(void) {

    // allocate the socket
    uv_tcp_t* tcp_sock = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	
    // initialize tcp handle
    int err = uv_tcp_init(loop, tcp_sock);
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



void net_start(void* args) {
    // create default loop
    loop = uv_default_loop();

    // initialize udp and tcp
    init_udp();
    init_tcp();
	
    // run the loop
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
}



