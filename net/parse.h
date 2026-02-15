#ifndef PARSE_H
#define PARSE_H

#include "../app.h"

raw_msg_list parse_messages(char* raw);
message parse_message(char* msg);
void print_messages(raw_msg_list* msg_list);
#endif
