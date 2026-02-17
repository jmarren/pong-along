#include "uv.h"
#include <stdlib.h>
#include <string.h>
#include "../client.h"

static uv_udp_t* udp_handle;
static struct sockaddr_in udp_dest;
static uv_loop_t* loop_ref;

/* PRIVATE */
/** 
 * callback to fire after sending udp message
 * frees request struct
 **/
void udp_send_cb(uv_udp_send_t* req, int status) { 
	free(req);
	
	if (status == -1) {
        	fprintf(stderr, "UDP send status = %d\n", status);
	}
}


/* PUBLIC  */
/* initializes udp for client application */
void udp_init(App* app) {
 
    // store a reference to the client application uv loop locally 
    // (for use in udp_write)
    loop_ref = app->loop;

    // allocate memory for the udp handle
    udp_handle = (uv_udp_t*)malloc(sizeof(uv_udp_t));


    // initialize udp_dest so we can send to it later
    int err = uv_ip4_addr("0.0.0.0", 8888, &udp_dest);

    if (err) {
        fprintf(stderr, "UDP bind error: %s\n", uv_strerror(err));
    }
}


/* writes to the udp socket */
void udp_write(char* message) {

    // create a uv buffer w/ provided message
    uv_buf_t buf;

    // save message to buffer
    buf.base = message;
    buf.len = strlen(message);

    // initialize udp handle
    int err = uv_udp_init(loop_ref, udp_handle);
    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

    // initialize udp send request
    uv_udp_send_t req;

    // send the buf to the udp_sock w/ udp_dest
    err = uv_udp_send(&req, udp_handle, &buf, 1, (const struct sockaddr*)&udp_dest, udp_send_cb);

    if (err) {
        fprintf(stderr, "UDP init error: %s\n", uv_strerror(err));
    }

}
