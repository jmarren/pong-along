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


typedef struct {
	SDL_Renderer* renderer;
	SDL_FRect rect_left;
	SDL_FRect rect_right;
	const SDL_Rect* dashboard_textbox;
	SDL_FRect dashboard_textbox_container;
	Circle circle;
	SDL_Window* window;
	bool game_started;
	TTF_Font * font;
	char* text_input;
} App;

#endif
