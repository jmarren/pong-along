#include "view/view.h"
#include "net/net.h"
#include "loop/loop.h"
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



int main(int argc, char *argv[])
{
	
	view_init();

	net_init();

	view_hello_world();


	pthread_t thread_id;
	// pthread_mutex_t mutex;

	// TODO:  Use specialized lib threads instead
  	int result = pthread_create(&thread_id, NULL, (void*)&net_start, NULL);

        if (result != 0) {
		perror("Failed to create thread");
		return 1;
	}

	printf("net_read_evt->type = %d\n", net_read_evt.type);


	loop_start();
	return 0;
}

