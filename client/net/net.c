
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stddef.h>
#include "tcp.h"
#include "udp.h"
#include "../client.h"


void net_start(App* app) {

    printf("net start\n");
    // create default loop
    app->loop = uv_default_loop();

    // initialize udp and tcp
    udp_init(app);
    tcp_init(app);
	
    // run the loop
    uv_run(app->loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
}



