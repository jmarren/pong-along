

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <stdio.h>
#include "app.h"
#include "net/parse.h"

#define TCP_PORT 7000
#define UDP_PORT 8888
#define BACKLOG 1000

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


void handle_selected_opponent(uv_stream_t* client, message* msg) {
	char* res = calloc(strlen(msg->content) + strlen("match: \r\n") + 1, sizeof(char));
	strncpy(res, "match: ", strlen("match: "));
	strncat(res, msg->content, strlen(msg->content));
	strncat(res, "\r\n", strlen("\r\n") + 1);
	// initialize the write buffer with the size of the message that was read (to echo back)
	uv_buf_t wrbuf = uv_buf_init(res, strlen(res) + 1);
		
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, write_callback);
}


void handle_username(uv_stream_t* client, message* msg) {
	// create a new user
	user user = {
		msg->content,
		client,
	};

	// add the user to active_users
	active_users.users[active_users.count] = user;
	active_users.count++;
}

void handle_players_query(uv_stream_t* client, message* msg) { 

	char* res = calloc(100, sizeof(char));

	strncpy(res, "players: ", strlen("players: "));
	
	for (int i = 0; i < active_users.count; i++) {
		if (active_users.users[i].stream != client) {
			strcat(res, active_users.users[i].username);
			strcat(res, ",");
		}
	}
	
	// append delimiter
	strcat(res, "\r\n");

	// initialize the write buffer with the size of the message that was read (to echo back)
	uv_buf_t wrbuf = uv_buf_init(res, 100);
		
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, write_callback);
}


void handle_tcp_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*) client, NULL);
        }
    } else if (nread > 0) {

	printf("received = %s\n", buf->base);
	fflush(stdout);
	raw_msg_list msg_list = parse_messages(buf->base);
	
	for (int i = 0; i < msg_list.count; i++) {
		message msg = parse_message(msg_list.messages[i]);

		if (strcmp(msg.type, "username") == 0) {
			handle_username(client, &msg);
		}

		if (strcmp(msg.type, "selected-opponent") == 0) {
			handle_selected_opponent(client, &msg);
		}

		if (strcmp(msg.content, "players?") == 0) {
			handle_players_query(client, &msg);
		}
	}
	fflush(stdout);

    } else {
	printf("read 0\n");
    }

    // free the client request buffer
    if (buf->base) {
        free(buf->base);
    }
}


void on_new_tcp_connection(uv_stream_t *server, int status) {
    // if error status, log error and return
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        return;
    }
	
    // allocate client 
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));

    // initialize tcp for client
    uv_tcp_init(loop, client);
	
    // accept connection and read if if no error is returned
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
	// pass the client (casted to uv_stream_t)
	// as well as a callback to allocate the buffer
	// and a function the handle the request
        uv_read_start((uv_stream_t*) client, alloc_buffer, handle_tcp_read);
    }
}


void on_udp_recv(uv_udp_t* req, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {

	   if (buf == NULL || nread == 0) return;

	   if (nread < 0) {
	     fprintf(stderr, "Read error %s\n", uv_err_name(nread));
	     uv_close((uv_handle_t*) req, NULL); // NULL is the uv_close_cb callback
	     free(buf->base);
	     return;
	   }

	   if (flags == UV_UDP_PARTIAL) {
		printf("GOT PARTIAL FLAG\n");
	   }

	   if (nread != 0) {
	     printf("on_read: %s\n", buf->base);
	     free(buf->base);
	   }
}

void init_udp(void) {

    int err = uv_udp_init(loop, &udp_server);
    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

    struct sockaddr_in udp_recv_addr;
    uv_ip4_addr("0.0.0.0", UDP_PORT, &udp_recv_addr);


    err = uv_udp_bind(&udp_server, (const struct sockaddr*)&udp_recv_addr, 0); 
    if (err) {
        fprintf(stderr, "UDP bind error: %s\n", uv_strerror(err));
    }


    // initialize callback to receive data on udp socket
    err = uv_udp_recv_start(&udp_server, alloc_buffer, on_udp_recv);
    // handle listen error
    if (err) {
        fprintf(stderr, "UDP recv error: %s\n", uv_strerror(err));
    }
}


void init_tcp(void) {
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

    init_tcp();
    init_udp();

    // run the loop with default flag
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

