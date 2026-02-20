#include "server.h"
#include "uv.h"
#include "udp.h"
#include "tcp.h"
#include <stdbool.h>


server_t server;


int main(void) {

    // create default libuv loop
    server.loop = uv_default_loop();

    server.active_users.len = 0;
	
    // initialize networking
    server_init_tcp(&server);
    server_init_udp(&server);

    // run the loop with default flag
    uv_run(server.loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

