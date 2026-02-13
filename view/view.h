
#ifndef VIEW_H
#define VIEW_H

#include <SDL3/SDL_render.h>
#include "../app.h"

#define WINDOW_W 2000
#define WINDOW_H 1000




void view_init(App* app);
void view_handle_up(void);
void view_handle_down(void);
void view_hello_world(void);
void view_render(App* app);

#endif

