#ifndef TCP_H
#define TCP_H

#include "uv.h"
#include <SDL3/SDL_stdinc.h>
#include "../app.h"

void net_write(char *message);
void tcp_init(App* app);

#endif


