#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#define MESSAGE "Hello Libuv\n"
#define HOST "127.0.0.1"
#define PORT 7000
uv_loop_t * loop;


char buffer[100];
// uv_buf_t* buf;
uv_connect_t *req;
// uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));



void handle_read(uv_stream_t *server, ssize_t nread, const uv_buf_t* buf) {
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
    uv_read_start(req->handle, alloc_buffer, handle_read);
}

void write_on_connection(char *message) {

    uv_buf_t buf;

    buf.base = malloc(sizeof(*message));
    buf.len = strlen(message);
    // buf->base = malloc(sizeof(char) * strlen(message));
    buf.base = message;
    uv_stream_t *tcp = req->handle;
    uv_write_t write_req;
    int buf_count = 1;
    printf("buf->base = %s\n", buf.base);

    uv_write(&write_req, tcp, &buf, buf_count, on_write_end);
}



void on_connect(uv_connect_t *new_req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }

   // alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)

    req = new_req;
    printf("connected\n");
}



int
SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    // CHECK_RENDERER_MAGIC(renderer, -1);

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int
SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    // CHECK_RENDERER_MAGIC(renderer, -1);

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


void start_uv_thread(void* args) {

    // create default loop
    uv_loop_t* loop = uv_default_loop();
	
    // allocate the socket
    uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	
    // initialize tcp
    uv_tcp_init(loop, socket);

    // allocate the connection struct
    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    // build the socket destination address struct
    struct sockaddr_in dest;
    uv_ip4_addr("0.0.0.0", 7000, &dest);

    // call connect using the on_connect callback
    uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);

    // run the loop
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
}



static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int main(int argc, char *argv[])
{

	SDL_InitSubSystem(0);

    	if (!SDL_CreateWindowAndRenderer("Hello World", 1000, 100, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
    	}


   	const char *message = "Hello World!";
   	int w = 0, h = 0;
   	float x, y;
   	const float scale = 1.0f;

   	/* Center the message and scale it up */
   	SDL_GetRenderOutputSize(renderer, &w, &h);
   	SDL_SetRenderScale(renderer, scale, scale);
   	x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
   	y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

   	/* Draw the message */
   	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
   	SDL_RenderClear(renderer);
   	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   	SDL_RenderDebugText(renderer, x, y, message);
   	SDL_RenderPresent(renderer);

	SDL_FRect rect_left;
	rect_left.x = 100;
	rect_left.y = 100;
	rect_left.w = 10;
	rect_left.h = 100;

	SDL_FRect rect_right;
	rect_right.x = 1000;
	rect_right.y = 100;
	rect_right.w = 10;
	rect_right.h = 100;

	pthread_t thread_id;
	// pthread_mutex_t mutex;

  	int result = pthread_create(&thread_id, NULL, (void*)&start_uv_thread, NULL);

        if (result != 0) {
		perror("Failed to create thread");
		return 1;
	}




	while (true) {
	 	SDL_Event event;

		    while (SDL_PollEvent(&event)) {  
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
				rect_left.y -= 36;
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				// SDL_RenderLine(renderer, 100, 100, 300, 300);
				SDL_RenderFillRect(renderer, &rect_left);
				SDL_RenderFillRect(renderer, &rect_right);
				SDL_RenderDrawCircle(renderer, 120, 120, 50);
				SDL_RenderFillCircle(renderer, 120, 120, 50);
				// SDL_RenderDebugText(renderer, x, y, "hi john");
				SDL_RenderPresent(renderer);

			}
			
			if (event.key.key == SDLK_DOWN) {	
				rect_left.y += 36;
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				// SDL_RenderLine(renderer, 100, 100, 300, 300);
				SDL_RenderFillRect(renderer, &rect_left);
				SDL_RenderFillRect(renderer, &rect_right);
				SDL_RenderDrawCircle(renderer, 120, 120, 50);
				SDL_RenderFillCircle(renderer, 120, 120, 50);
				// SDL_RenderDebugText(renderer, x, y, "hi john");
				SDL_RenderPresent(renderer);

			}
			
			if (event.key.key == SDLK_H) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				// SDL_RenderLine(renderer, 100, 100, 300, 300);
				// rect.x++;
				
				SDL_RenderFillRect(renderer, &rect_left);
				// SDL_RenderDebugText(renderer, x, y, "hi john");
				SDL_RenderPresent(renderer);
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
