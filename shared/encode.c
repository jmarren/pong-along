

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  encodes type and content to protocol format   */
char* encode_message(char* type, char* content) {
	size_t len = strlen(type) + strlen(content) + strlen("\r\n");

	// allocate the full request string
	char* str = calloc(len + 1, sizeof(char));

	// copy type and content to the request string
	sprintf(str, "%s:%s\r\n", type, content);

	return str;

}


