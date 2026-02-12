
#ifndef VIEW_CIRCLE_H
#define VIEW_CIRCLE_H

#include "physics.h"
#include <SDL3/SDL_render.h>

typedef struct {
	Object obj;
	int radius;
} Circle;


Circle* circle_init(void);
void circle_render(SDL_Renderer * renderer);
// int view_draw_circle(SDL_Renderer * renderer);
// int view_fill_circle(SDL_Renderer * renderer);
void circle_move(SDL_Renderer * renderer);

#endif


