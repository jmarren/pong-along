#include "uv.h"
#include <stdlib.h>
#include <string.h>
#include "../app.h"

uv_udp_t* udp_sock;
struct sockaddr_in udp_dest;
uv_loop_t* loop_ref;


void udp_send_cb(uv_udp_send_t* req, int status) { 
	free(req);
	
	if (status == -1) {
        	fprintf(stderr, "UDP send status = %d\n", status);
	}
}



void init_udp(App* app) {
 
    loop_ref = app->loop;


    udp_sock = (uv_udp_t*)malloc(sizeof(uv_udp_t));


    // initialize udp_dest so we can send to it later
    int err = uv_ip4_addr("0.0.0.0", 8888, &udp_dest);

    if (err) {
        fprintf(stderr, "UDP bind error: %s\n", uv_strerror(err));
    }
}



void write_udp(char* message) {

    // create a uv buffer w/ provided message
    uv_buf_t buf;
    buf.base = message;
    buf.len = strlen(message) + 1;
    int buf_count = 1;

    // initialize udp handle
    int err = uv_udp_init(loop_ref, udp_sock);
    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

    // struct sockaddr_in udp_destination;
    uv_udp_send_t req;

    // send the buf to the udp_sock w/ udp_dest
    err = uv_udp_send(&req, udp_sock, &buf, buf_count, (const struct sockaddr*)&udp_dest, udp_send_cb);

    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

}
