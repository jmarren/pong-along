
#include "client.h"
#include "renderer.h"
#include "net/net.h"
#include "loop.h"
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
#include <stdlib.h>
#include <uv.h>
#include "frames/enter_username.h"
#include "frames/select_opponent.h"
#include "frames/pointing.h"
#include "frames/gameplay.h"


#define HOST "127.0.0.1"
#define PORT 7000
#define ACTIVE_USERS_CAP 50

App app;

int main(int argc, char *argv[])
{

	app.current_frame = initializing;

	app.read_event_type = SDL_RegisterEvents(1);

	init_renderer(&app);

	app.handlers[enter_username] = h_enter_username;
	app.handlers[select_opponent] = h_select_opponent;
	app.handlers[gameplay] = h_gameplay;


	pthread_t thread_id;

	// TODO:  Use specialized lib threads instead
  	int result = pthread_create(&thread_id, NULL, (void*)&net_start, &app);

        if (result != 0) {
		perror("Failed to create thread");
		return 1;
	}


	app.current_frame = enter_username; 


	loop_start(&app);
	return 0;
}

