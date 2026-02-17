

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <stdio.h>
#include "app.h"
#include "net/parse.h"


uv_tcp_t tcp_server;
uv_udp_t udp_server;
uv_loop_t* loop;

users_list active_users;





void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = calloc(1, suggested_size);
    buf->len = suggested_size;
}

void write_callback(uv_write_t *req, int status) {
    if (status) {
	printf("write error\n");
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
	
    free(req);
}

void get_other_users(uv_stream_t* client, users_list* other_users) {
	
	other_users->count = 0;
	other_users->users = malloc(sizeof(user) * 10);

	for (int i = 0; i < active_users.count; i++) {
		user user = active_users.users[i];
		if (user.stream != client) {
			other_users->users[other_users->count] = user;
			other_users->count++;
		}
	}
}







void server_init_tcp(void) {
    int err;
    // initialize tcp server with loop and pointer to server
    err = uv_tcp_init(loop, &tcp_server);
    if (err) {
        fprintf(stderr, "TCP init error: %s\n", uv_strerror(err));
    }

    // create the socket address and assign it host & port 
    struct sockaddr_in tcp_addr;
    uv_ip4_addr("127.0.0.1", TCP_PORT, &tcp_addr);


    // bind the server to the socket address
    err = uv_tcp_bind(&tcp_server, (const struct sockaddr*)&tcp_addr, UV_TCP_REUSEPORT); 
    if (err) {
        fprintf(stderr, "TCP bind error: %s\n", uv_strerror(err));
    }


    // listen on the address with the on_new_connection callback
    err = uv_listen((uv_stream_t*) &tcp_server, BACKLOG, on_new_tcp_connection);

    // handle listen error
    if (err) {
        fprintf(stderr, "TCP Listen error: %s\n", uv_strerror(err));
    }
}


// Callbacks (on_new_connection, on_read, on_write, on_alloc, etc.) 
// must be defined to handle specific events.
int main(void) {

    active_users.users = malloc(sizeof(user) * 10);
    active_users.count = 0;

    // create default libuv loop
    loop = uv_default_loop();

    server_init_tcp();
    server_init_udp();

    // run the loop with default flag
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

