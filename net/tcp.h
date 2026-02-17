#ifndef TCP_H
#define TCP_H

#include <SDL3/SDL_stdinc.h>
#include "../app.h"
#include "../shared/parse.h"

// void tcp_write(char *message);
void tcp_init(App* app);
void tcp_write_msg_2(message* msg);
void tcp_write_msg_1(char* type, char* content);

#endif


