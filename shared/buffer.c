#include "uv.h"
#include <stdlib.h>
#include <string.h>
#include "encode.h"

void buffer_alloc_uv_handle(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = calloc(1, suggested_size);
    buf->len = suggested_size;
}


void buffer_assign(char *message, uv_buf_t* buf) {
    // set length of buffer to message length
    buf->len = strlen(message);
    // assign base to message
    buf->base = message;

}

void buffer_encode_assign(char* type,  char* content, uv_buf_t* buf) {

	char* encoded = encode_message(type, content);

	// assign string to write buffer
        buffer_assign(encoded, buf);
}
