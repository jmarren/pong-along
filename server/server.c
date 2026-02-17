
#include "server.h"
#include "uv.h"
#include <stdlib.h>
#include "udp.h"
#include "tcp.h"


server_t server;


// Callbacks (on_new_connection, on_read, on_write, on_alloc, etc.) 
// must be defined to handle specific events.
int main(void) {

    server.active_users.users = malloc(sizeof(user) * 10);
    server.active_users.count = 0;

    // create default libuv loop
    server.loop = uv_default_loop();

    server_init_tcp(&server);
    server_init_udp(&server);

    // run the loop with default flag
    uv_run(server.loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

