#ifndef MAIN_H
#define MAIN_H


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
	Circle circle;
	SDL_Window* window;
	bool game_started;
} App;

#endif
