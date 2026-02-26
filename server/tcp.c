


#include "uv.h"
#include "server.h"
#include <stdlib.h>
#include <string.h>
#include "../shared/parse.h"
#include "../shared/buffer.h"
#include "../shared/callback.h"
#include "handlers/handlers.h"

#define BACKLOG 1000
#define TCP_PORT 7000

static server_t* server;
 

void write_tcp(uv_stream_t* client, char* type, char* content) {
	uv_buf_t wrbuf;
	// encode to protocol format and assign to write buffer
	buffer_encode_assign(type, content, &wrbuf);
	
	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
	uv_write(req, client, &wrbuf, 1, cb_free_write);

}

void handle_tcp_read(uv_stream_t* client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*) client, NULL);
        }
    } else if (nread > 0) {

	raw_msg_list msg_list = parse_messages(buf->base);
	
	for (int i = 0; i < msg_list.count; i++) {
		message msg = parse_message(msg_list.messages[i]);

		printf("got msg.type = %s\n", msg.type);

		if (strcmp(msg.type, "username") == 0) {
			handle_username(server, client, &msg);
		}

		if (strcmp(msg.type, "selected-opponent") == 0) {
			handle_selected_opponent(server, client, &msg);
		}

		if (strcmp(msg.type, "query") == 0) {
			handle_players_query(server, client, &msg);
		}
	}

    } else {
	printf("read 0\n");
    }

    // free the client request buffer
    if (buf->base) {
        free(buf->base);
    }
}


void on_new_tcp_connection(uv_stream_t *stream, int status) {
    // if error status, log error and return
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        return;
    }
	
    // allocate client 
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));

    // initialize tcp for client
    uv_tcp_init(server->loop, client);
	
    // accept connection and read if if no error is returned
    if (uv_accept(stream, (uv_stream_t*) client) == 0) {
	// pass the client (casted to uv_stream_t)
	// as well as a callback to allocate the buffer
	// and a function the handle the request
        uv_read_start((uv_stream_t*) client, buffer_alloc_uv_handle, handle_tcp_read);
    }
}


void server_init_tcp(server_t* server_ref) {
    server = server_ref;
    uv_tcp_t* tcp_server = &(server_ref->tcp_server);
    uv_loop_t* loop_ref = server_ref->loop;

    // initialize tcp server with loop and pointer to server
    int err = uv_tcp_init(loop_ref, tcp_server);
    if (err) {
        fprintf(stderr, "TCP init error: %s\n", uv_strerror(err));
    }

    // create the socket address and assign it host & port 
    struct sockaddr_in tcp_addr;
    uv_ip4_addr("127.0.0.1", TCP_PORT, &tcp_addr);


    // bind the server to the socket address
    err = uv_tcp_bind(tcp_server, (const struct sockaddr*)&tcp_addr, UV_TCP_REUSEPORT); 
    if (err) {
        fprintf(stderr, "TCP bind error: %s\n", uv_strerror(err));
    }


    // listen on the address with the on_new_connection callback
    err = uv_listen((uv_stream_t*) tcp_server, BACKLOG, on_new_tcp_connection);

    // handle listen error
    if (err) {
        fprintf(stderr, "TCP Listen error: %s\n", uv_strerror(err));
    }
}


