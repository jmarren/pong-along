
#include "client.h"
#include "view/view.h"
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


#define HOST "127.0.0.1"
#define PORT 7000
#define ACTIVE_USERS_CAP 50

App app;

int main(int argc, char *argv[])
{

	app.game_phase = initializing;
	app.active_users.capacity = ACTIVE_USERS_CAP;
	app.active_users.base = (char**)calloc(ACTIVE_USERS_CAP, sizeof(char*));
	app.active_users.len = 0;
	app.text_input = (char*)calloc(100, sizeof(char));
	app.username = (char*)calloc(100, sizeof(char));

	app.read_event_type = SDL_RegisterEvents(1);

	view_init(&app);


	pthread_t thread_id;

	// TODO:  Use specialized lib threads instead
  	int result = pthread_create(&thread_id, NULL, (void*)&net_start, &app);

        if (result != 0) {
		perror("Failed to create thread");
		return 1;
	}


	app.game_phase = typing; 

	SDL_StartTextInput(app.window);

	loop_start(&app);
	return 0;
}

