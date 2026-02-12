
#ifndef VIEW_H
#define VIEW_H

#include "circle.h"
#include <SDL3/SDL_render.h>
#define WINDOW_W 1000
#define WINDOW_H 1000



typedef struct {
	SDL_Renderer* renderer;
	SDL_FRect* rect_left;
	SDL_FRect* rect_right;
	Circle* circle;
	bool game_started;
} App;


App* view_init(void);
void view_circle_move(void);
void view_handle_up(void);
void view_handle_down(void);
void view_handle_h(void);
void view_hello_world(void);
void view_render(void);

#endif

