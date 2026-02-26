
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <stdlib.h>
#include "../client.h"
#include "../view/text.h"
#include "../net/tcp.h"
#include "../view/rect.h"
#define TITLE_H 10
#define TITLE_TEXT "enter your username"

// text_input rect
static SDL_FRect text_input_rect = (SDL_FRect){
		.x = FR_MARGIN_LEFT,
		.y = FR_MARGIN_TOP + TITLE_H + 10,
		.w = TEXTBOX_WIDTH,
		.h = TEXTBOX_HEIGHT,
};

// initialize text input component
static void init_text_input(fr_enter_username* frame) {
	char* input_text = calloc(10, sizeof(char));
	frame->input_component = (text_component){
		.rect = text_input_rect,
		.text = input_text,
	};
}

// initialize text input
static void init_title(fr_enter_username* frame) {
	frame->title_component = (text_component){
		.rect = title_rect,
		.text = TITLE_TEXT,
	};
}


static void handle_backspace(fr_enter_username* frame) {
	int len = strlen(frame->input_component.text);
	if (len > 0) {
		frame->input_component.text[len - 1] = '\0';
	}
}

static void handle_return(App* app) {
	// stop taking input
	SDL_StopTextInput(app->window);

	// copy text_input to username
	strncpy(app->username, app->frames.enter_username.input_component.text, 25);

	// set game phase to pointing
	app->current_frame = select_opponent;
	
	tcp_write_msg_1("username", app->username);

	tcp_write_msg_1("query", "players");
}

static void handle_text_input(fr_enter_username* frame, SDL_Event* event) {
	int len = strlen(frame->input_component.text);
	if (len < MAX_USERNAME_CHARS) {
		strcat(frame->input_component.text, event->text.text);
	}
}


static void init(App* app) {
	printf("enter_username init\n");
	fr_enter_username* fr = &(app->frames.enter_username);
	SDL_StartTextInput(app->window);
	init_text_input(fr);
	init_title(fr);
}


static void input(App* app, SDL_Event* event) {
	
	fr_enter_username* fr = &(app->frames.enter_username);

	if (event->type == SDL_EVENT_KEY_DOWN) {
		SDL_Keycode key = event->key.key;

		switch (key) {
			case SDLK_BACKSPACE:
				handle_backspace(fr);
				break;
			case SDLK_RETURN:
				handle_return(app);
				break;
		}
	}
	if (event->type == SDL_EVENT_TEXT_INPUT) {
		handle_text_input(fr,event);
	}
}



static void render(App* app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	text_render(app, &(app->frames.enter_username.title_component));
	text_render(app, &(app->frames.enter_username.input_component));
}





extern fr_handler h_enter_username = (fr_handler){
	.init = (init_handler*)init,
	.input = (input_handler*)input,
	.render = (render_handler*)render,
};




