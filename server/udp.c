
#include "uv.h"
#include <stdlib.h>
#include "server.h"
#include "shared.h"

#define UDP_PORT 8888

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

void server_init_udp(server_t* server) {
  
    uv_loop_t* loop = server->loop;
    uv_udp_t* udp_server = &(server->udp_server);

    int err = uv_udp_init(loop, udp_server);
    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

    struct sockaddr_in udp_recv_addr;
    uv_ip4_addr("0.0.0.0", UDP_PORT, &udp_recv_addr);


    err = uv_udp_bind(udp_server, (const struct sockaddr*)&udp_recv_addr, 0); 
    if (err) {
        fprintf(stderr, "UDP bind error: %s\n", uv_strerror(err));
    }


    // initialize callback to receive data on udp socket
    err = uv_udp_recv_start(udp_server, alloc_buffer, on_udp_recv);
    // handle listen error
    if (err) {
        fprintf(stderr, "UDP recv error: %s\n", uv_strerror(err));
    }
}


