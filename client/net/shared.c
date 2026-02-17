


#include "uv.h"
#include <stdlib.h>
void buffer_alloc_uv_handle(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}
