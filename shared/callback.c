



#include "uv.h"
#include <stdlib.h>

void cb_free_write(uv_write_t *req, int status) {
    if (status) {
	printf("write error\n");
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
	
    free(req);
}
