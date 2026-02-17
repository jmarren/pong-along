
#include <stdio.h>
#include "parse.h"
#include <stdlib.h>
#include <string.h>


void print_raw_messages(raw_msg_list* msg_list) {
	printf("raw messages: \n");
	for (int i = 0; i < msg_list->count; i++) {
		printf("%s\n", msg_list->messages[i]);
	}
	fflush(stdout);
}

void print_messages(message_list* msg_list) {
	printf("messages: \n");
	for (int i = 0; i < msg_list->len; i++) {
		printf("type = %s\ncontent = %s\n", msg_list->base[i].type, msg_list->base[i].content);
	}
	fflush(stdout);
}




raw_msg_list parse_messages(char* raw) {
	// initialize raw message list
	raw_msg_list msgs;
	msgs.count = 0;
	msgs.messages = calloc(100, sizeof(char*));
	
	
	char* token = strtok(raw, "\r\n");

	while (token != NULL) {
		msgs.messages[msgs.count] = calloc(1, strlen(token));
		msgs.messages[msgs.count] = token;
		msgs.count++;
		token = strtok(NULL, "\r\n");
	}


	return msgs;
}



message parse_message(char* msg) {
	message message;

	char* type = strtok(msg, ":");
	char* content = strtok(NULL, ":");

	message.type = type;
	message.content = content;
	return message;
}

message_list parse_raw_message_list(raw_msg_list* list) {
	message_list msg_list;
	msg_list.len = 0;

	for (int i = 0; i < list->count; i++) {
		message msg;
		msg = parse_message(list->messages[i]);
		msg_list.base[i] = msg;
		msg_list.len++;
	}
	
	
	return msg_list;
}

