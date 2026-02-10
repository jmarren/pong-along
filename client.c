

#include "uv.h"
#include <stdlib.h>
#include <string.h>

#define PORT 7000
uv_loop_t * loop;
// typedef void (*uv_connect_cb)(uv_connect_t* req, int status);

void write_to_conn(uv_write_t* req, int status) {

	// free buffer
}

void echo_read(uv_stream_t *server, ssize_t nread, const uv_buf_t* buf) {
    if (nread == -1) {
        fprintf(stderr, "error echo_read");
        return;
    }

    printf("result: %s\n", buf->base);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_write_end(uv_write_t *req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
    uv_read_start(req->handle, alloc_buffer, echo_read);
}

// void alloc_buffer(uv_buf_t* buf) {
//     buf->base = (char*) malloc(sizeof(char) * 100);
//     buf->len = 100;
// }


void on_connect(uv_connect_t * req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
    printf("connected\n");
    char buffer[100];
    uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));
    char *message = "hello";
    buf.len = strlen(message);
    buf.base = message;
    uv_stream_t *tcp = req->handle;
    uv_write_t write_req;
    int buf_count = 1;
    uv_write(&write_req, tcp, &buf, buf_count, on_write_end);
}


// struct uv_write_s {
//   UV_REQ_FIELDS
//   uv_write_cb cb;
//   uv_stream_t* send_handle; /* TODO: make private and unix-only in v2.x. */
//   uv_stream_t* handle;
//   UV_WRITE_PRIVATE_FIELDS
// };
/* uv_connect_t is a subclass of uv_req_t. */
// struct uv_connect_s {
//   UV_REQ_FIELDS
//   uv_connect_cb cb;
//   uv_stream_t* handle;
//   UV_CONNECT_PRIVATE_FIELDS
// };

// void on_connect(uv_connect_t* req, int status) {
// 	// uv_write(uv_write_t *req, uv_stream_t *handle, const uv_buf_t *bufs, unsigned int nbufs, uv_write_cb cb)
// 	uv_write_t * write;
// 	uv_buf_t * buf;
//
// 	alloc_buffer(buf);
//
// 	buf->base = "hi from client";
//
//
// // UV_EXTERN int uv_write(uv_write_t* req,
// //                        uv_stream_t* handle,
// //                        const uv_buf_t bufs[],
// //                        unsigned int nbufs,
// //                        uv_write_cb cb);
// 	uv_write((uv_connect_t *)write, req, buf, 1, write_to_conn);
//
// 	printf("connected\n");
// }

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, socket);

    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    struct sockaddr_in dest;
    uv_ip4_addr("0.0.0.0", 7000, &dest);

    uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);
	
    return uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

}

// int main() {
//
//     loop = malloc(sizeof(uv_loop_t));
//     uv_loop_init(loop);
//
//     // uv_run(loop, UV_RUN_DEFAULT);
//     // uv_tcp_init(loop, &server); // 2. Initialize the TCP Handle
//
//     struct sockaddr_in addr;
//     uv_ip4_addr("127.0.0.1", PORT, &addr);
//
//     uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
//     uv_tcp_init(loop, socket);
//
//     uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
//
//     struct sockaddr_in dest;
//     uv_ip4_addr("127.0.0.1", 80, &dest);
//
//     uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);
//
//
//     // uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0); // 3. Bind to address
//     //
//     // int r = uv_listen((uv_stream_t*) &server, BACKLOG, on_new_connection); // 4. Start Listening
//     // if (r) {
//     //     fprintf(stderr, "Listen error: %s\n", uv_strerror(r));
//     //     return 1;
//     // }
//
//     return uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
// }
