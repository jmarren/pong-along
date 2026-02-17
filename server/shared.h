
#ifndef SERVER_SHARED_H
#define SERVER_SHARED_H

#include "uv.h"
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

#endif
