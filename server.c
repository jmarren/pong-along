

#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <stdio.h>
// #include <stdlib.h>

#define PORT 7000
#define BACKLOG 1000

uv_tcp_t server;
uv_loop_t * loop;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void write_callback(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
	
    free(req);
}

void handle_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*) client, NULL);
        }
    } else if (nread > 0) {
	// print the message
	printf("message: %s\n", buf->base);

	// allocate the write request to write back
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

	// initialize the write buffer with the size of the message that was read (to echo back)
	uv_buf_t wrbuf = uv_buf_init(buf->base, nread);

	// write the message back to the client
	// with buf len of 1 and a write callback that 
	// frees the write request
        uv_write(req, client, &wrbuf, 1, write_callback);
    } else {
	printf("read 0\n");
    }

    // free the client request buffer
    if (buf->base) {
        free(buf->base);
    }
}

void on_new_connection(uv_stream_t *server, int status) {
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
        uv_read_start((uv_stream_t*) client, alloc_buffer, handle_read);
    }
}




// Callbacks (on_new_connection, on_read, on_write, on_alloc, etc.) 
// must be defined to handle specific events.
int main() {
    // create default libuv loop
    loop = uv_default_loop();

    // initialize tcp server with loop and pointer to server
    uv_tcp_init(loop, &server);

    // create the socket address and assign it host & port 
    struct sockaddr_in addr;
    uv_ip4_addr("127.0.0.1", PORT, &addr);

    // bind the server to the socket address
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, UV_TCP_REUSEPORT); 

    // listen on the address with the on_new_connection callback
    int err = uv_listen((uv_stream_t*) &server, BACKLOG, on_new_connection);

    // handle listen error
    if (err) {
        fprintf(stderr, "Listen error: %s\n", uv_strerror(err));
        return 1;
    }

    // run the loop with default flag
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

