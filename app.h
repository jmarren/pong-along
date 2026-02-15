#ifndef MAIN_H
#define MAIN_H


#include "vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "view/physics.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

typedef struct {
	Object obj;
	int radius;
} Circle;


typedef enum {
	initializing,
	typing,
	playing,
	pointing,
} game_phase;


typedef struct {
	SDL_Renderer* renderer;
	SDL_FRect rect_left;
	SDL_FRect rect_right;
	SDL_FRect dashboard_textbox;
	SDL_FRect dashboard_title;
	SDL_FRect gameplay_header;
	Uint32 read_event_type;
	Circle circle;
	char* username;
	SDL_Window* window;
	game_phase game_phase;
	// bool game_started;
	TTF_Font * font;
	char* text_input;
	SDL_FRect text_input_rect;
} App;

#endif
