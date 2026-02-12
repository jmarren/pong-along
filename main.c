#include "view/view.h"
#include "net/net.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <stdint.h>
#include <stdio.h>
#include <uv.h>
#define MESSAGE "Hello Libuv\n"
#define HOST "127.0.0.1"
#define PORT 7000


uv_loop_t * loop;
// TODO: use SDL_UserEvent instead
char buffer[100];

// SDL_Event net_read_evt;


int main(int argc, char *argv[])
{
	
	// SDL_Event* read_event;
	// read_event = init_views();
		
		

	init_views();

	init_net();

	hello_world();


	pthread_t thread_id;
	// pthread_mutex_t mutex;

	// TODO:  Use specialized lib threads instead
  	int result = pthread_create(&thread_id, NULL, (void*)&start_uv_thread, NULL);

        if (result != 0) {
		perror("Failed to create thread");
		return 1;
	}

	printf("net_read_evt->type = %d\n", net_read_evt.type);


	while (true) {

	 	SDL_Event event;

		    while (SDL_PollEvent(&event)) {  
			
			if (event.type == net_read_evt.type) {
				printf("uv event fired\n");
			}
		

			// poll until all events are handled!
			// decide what to do with this event.

			if (event.type == SDL_EVENT_QUIT){
				goto end;
			}

			SDL_Keycode key = event.key.key;
			if (key == SDLK_ESCAPE) {
				goto end;
			}

	
			if (event.key.key == SDLK_UP) {	
				handle_up();		
			}
			
			if (event.key.key == SDLK_DOWN) {	
				handle_down();
			}
			
			if (event.key.key == SDLK_H) {
				handle_h();
			}

			if (key == SDLK_W) {
				write_on_connection("hi\r\n");			
			}
			if (key == SDLK_K) {
				char* msg = "bye\r\n";
				printf("msg = %s\n", msg);
				write_on_connection(msg);			
			}
		}
	}

	end:

	return 0;
}



// uv_loop_t *loop;
//
// // Callback for memory allocation
// void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
//     buf->base = (char*) malloc(suggested_size);
//     buf->len = suggested_size;
// }
//
// // Callback after data is read
// void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
//     if (nread > 0) {
//         // Process received data
//         printf("Received: %s", buf->base);
//     } else if (nread < 0) {
//         if (nread != UV_EOF) {
//             fprintf(stderr, "Read error: %s\n", uv_strerror(nread));
//         }
//         // Close the handle on error or EOF
//         uv_close((uv_handle_t*) stream, NULL);
//     }
//     // Free the buffer allocated by alloc_buffer
//     if (buf->base) {
//         free(buf->base);
//     }
// }
//
// // Callback after data is written
// void on_write(uv_write_t *req, int status) {
//     if (status) {
//         fprintf(stderr, "Write error: %s\n", uv_strerror(status));
//     }
//     // Writing is done, start reading the response
//     uv_read_start(req->handle, alloc_buffer, on_read);
//     // Free the write request memory
//     free(req);
// }
//
// // Callback after connection is established
// void on_connect(uv_connect_t *req, int status) {
//     if (status < 0) {
//         fprintf(stderr, "Connection error: %s\n", uv_strerror(status));
//         uv_close((uv_handle_t*) req->handle, NULL);
//         free(req);
//         return;
//     }
//
//     printf("Connected to server. Sending message...\n");
//
//     // Prepare data to send
//     uv_buf_t buf = uv_buf_init(MESSAGE, strlen(MESSAGE));
//     uv_write_t *write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
//
//     // Send data asynchronously
//     uv_write(write_req, req->handle, &buf, 1, on_write);
//
//     // Free the connect request memory (the handle is now managed by subsequent callbacks)
//     free(req); 
// }
//
