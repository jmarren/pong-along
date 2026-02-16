#ifndef NET_SHARED_H
#define NET_SHARED_H

#include "uv.h"
#include <stddef.h>

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
#endif


