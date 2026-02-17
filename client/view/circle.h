
#ifndef VIEW_CIRCLE_H
#define VIEW_CIRCLE_H

#include <SDL3/SDL_render.h>
#include "../client.h"


void circle_init(App* app);
void circle_render(App* app);
void circle_bounce_wall_left_right(App* app);
void circle_move(App* app);

#endif


