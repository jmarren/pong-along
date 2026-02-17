
#ifndef PARSE_H
#define PARSE_H



typedef struct {
	char* type;
	char* content;
} message;


typedef struct {
	char** messages;
	int count;
} raw_msg_list;

typedef struct {
	message* base;
	int len;
} message_list;

raw_msg_list parse_messages(char* raw);
message parse_message(char* msg);
void print_raw_messages(raw_msg_list* msg_list);
message_list parse_raw_message_list(raw_msg_list* list);
void print_messages(message_list* msg_list);
#endif
