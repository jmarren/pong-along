#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H
#include "uv.h"
void buffer_alloc_uv_handle(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void buffer_assign(char *message, uv_buf_t* buf);
void buffer_encode_assign(char* type,  char* content, uv_buf_t* buf);
#endif
