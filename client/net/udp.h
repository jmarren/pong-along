

#ifndef UDP_H
#define UDP_H

#include "../client.h"

void udp_init(App* app);
void udp_write(char* message);
void udp_write_msg_1(char* type, char* content);

#endif
