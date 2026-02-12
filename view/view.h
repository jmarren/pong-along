
#ifndef VIEW_H
#define VIEW_H

#include <SDL3/SDL_render.h>

void init_views(void);
void handle_up(void);
void handle_down(void);
void handle_h(void);
void hello_world(void);

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);

#endif

