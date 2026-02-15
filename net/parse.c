#include "../app.h"
#include <stdio.h>
#include <stdlib.h>


void print_messages(raw_msg_list* msg_list) {
	printf("messages: \n");
	for (int i = 0; i < msg_list->count; i++) {
		printf("%s\n", msg_list->messages[i]);
	}
	fflush(stdout);
}




raw_msg_list parse_messages(char* raw) {
	raw_msg_list msgs;
	msgs.count = 0;
	msgs.messages = calloc(50, sizeof(char*));
	
	
	int indices_size = 10;
	
	int* parse_indices = calloc(indices_size, sizeof(int));
	int num_indices = 0;

	// get indices of /r/n delimiter
	for (int i = 0; i < (int)strlen(raw) - 1; i++) {
		if (raw[i] == '\r' && raw[i+1] == '\n') {
			parse_indices[num_indices] = i;
			num_indices++;
			
			// reallocate if necessary
			if (num_indices > indices_size) { 
				indices_size *= 2;
				parse_indices = realloc(parse_indices, indices_size);
			}
		} 
	}
	
	int start_index = 0;

	
	// copy contents into msgs
	for (int i = 0; i < num_indices; i++) {
		fflush(stdout);
		// calculate message length
		int len = parse_indices[i] - start_index;
	
		// allocate space for the message in the message list
		msgs.messages[msgs.count] = calloc(sizeof(char), len);
	
		// copy (len) bytes from the start index into the last message in the list
		strncpy(msgs.messages[msgs.count], &raw[start_index], len);

		// increment message count
		msgs.count++;
		
		// set the start index to the start of the next message (exlude \r\n)
		start_index = parse_indices[i] + 2;
	}


	return msgs;
}




// void trim_message(char* msg) {
// 	for (int i = 0; i < (int)strlen(msg); i++) {
// 		if (msg[i] == '\r') {
// 			msg[i] = '\0';
// 		}
// 	}
// }

message parse_message(char* msg) {
	message message;
	
	int colon_index = 0;
	bool found = false;

	for (int i = 0; i < (int)strlen(msg); i++) {
		if (msg[i] == ':') {
			colon_index = i;
			found = true;
			break;
		} 
	}

	if (!found) {
		message.type = "unknown";
		message.content = msg;
		return message;
	}

	
	size_t type_len = colon_index * sizeof(char) + 1;
	size_t content_len = strlen(msg) - type_len + 1; 

	message.type = (char*)calloc(sizeof(char), type_len);
	message.content = (char*)calloc(sizeof(char), content_len);

	strncpy(message.type, msg, colon_index);
	strncpy(message.content, &msg[colon_index + 2], content_len - 1);
	
	return message;
}

message_list parse_raw_message_list(raw_msg_list* list) {
	

}

