#ifndef PARSE_H
#define PARSE_H

#include "../app.h"

raw_msg_list parse_messages(char* raw);
message parse_message(char* msg);
void print_raw_messages(raw_msg_list* msg_list);
message_list parse_raw_message_list(raw_msg_list* list);
void print_messages(message_list* msg_list);
#endif
