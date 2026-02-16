#ifndef TCP_H
#define TCP_H

#include <SDL3/SDL_stdinc.h>
#include "../app.h"

void tcp_write(char *message);
void tcp_init(App* app);

#endif


