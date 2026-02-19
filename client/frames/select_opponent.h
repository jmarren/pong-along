#ifndef SELECT_OPPONENT_H
#define SELECT_OPPONENT_H
#include "../client.h"
#include <SDL3/SDL_events.h>
void select_opponent_init(App* app);
void select_opponent_render(App* app);
void select_opponent_input(App* app, SDL_Event* event);
#endif
