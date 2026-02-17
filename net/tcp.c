
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stdlib.h>
#include "../app.h"
#include "shared.h"
#include "../shared/parse.h"

/* ----------------- PRIVATE ------------------ */
static uv_connect_t *req;
static Uint32 read_event_type;


/**
 * callback to fire after writing to tcp connection 
 * frees write request struct
 **/
void on_write_end(uv_write_t *write_req, int status) {
    // handle failed status
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }

    // free write request
    free(write_req);
}

/**
 * callback to fire when data is read from the tcp connection 
 * creates an SDL_UserEvent with registered read event type
 * and pushes it to the SDL loop with message copied to event.data1
 **/
void read_data(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {

   // if < 0 bytes read, log and close the connection
   if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*)stream, NULL);
        }
	return;
   }

   // if == 0 bytes read, log and return
   if (nread == 0) {
	   printf("nread = 0\n");
	   return;
   }

   // if nread > 0 copy to SDL_UserEvent and push to SDL loop
   if (nread > 0) {
	// initialize a user event
	SDL_UserEvent event;
	
	// set event type 
	event.type = read_event_type;
	
	// set data1 to the buffer base pointer
	event.data1 = (void*)buf->base;
	
	// push event to SDL loop
    	SDL_PushEvent((SDL_Event*)&event);
    }
}


/* write a string to the tcp connection  */
void tcp_write(char *message) {

    // initialize write buffer
    uv_buf_t buf;

    // set length of buffer to message length
    buf.len = strlen(message);

    // assign base to message
    buf.base = message;

    // allocate write request
    uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));

    // perform write 
    uv_write(write_req, req->handle, &buf, 1, on_write_end);
}


/* ----------------- PUBLIC ------------------ */

/* write a message type and content to the connection */
void tcp_write_msg_1(char* type, char* content) {
	// calculate size of full request string
	size_t len = strlen(type) + strlen(content) + strlen("\r\n");

	// allocate the full request string
	char* str = calloc(len + 1, sizeof(char));

	// copy type and content to the request string
	sprintf(str, "%s:%s\r\n", type, content);

	// write to connection
	tcp_write(str);

}

/* writes a message to the tcp connection  */
void tcp_write_msg_2(message* msg) {

	// calculate size of full request string
	size_t len = strlen(msg->type) + strlen(msg->content) + strlen("\r\n");

	// allocate the request string
	char* str = calloc(len + 1, sizeof(char));

	// copy type and content to the request string
	sprintf(str, "%s:%s\r\n", msg->type, msg->content);

	// write request string to connection
	tcp_write(str);
}



/* handles a new tcp connection to the server */
void on_connect(uv_connect_t *new_req, int status) {
    // handle failed status
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
	
    // store a reference to the request locally
    req = new_req;

    // start the read callback
    uv_read_start(req->handle, alloc_buffer, read_data);
}


/**
 * initialize tcp for the client application
 *
 * store read event type locally
 * init tcp handle
 * connect to server destination address
 **/
void tcp_init(App* app) {

    // save a reference to the read event type locally
    read_event_type = app->read_event_type;


    // allocate the socket
    uv_tcp_t* tcp_sock = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	
    // initialize tcp handle
    int err = uv_tcp_init(app->loop, tcp_sock);
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
