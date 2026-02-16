
#include "uv.h"
#include <SDL3/SDL_events.h>
#include <stddef.h>
// #include <stdlib.h>
#include "tcp.h"
#include "udp.h"
#include "../app.h"


void net_start(App* app) {
    // create default loop
    app->loop = uv_default_loop();

    // initialize udp and tcp
    init_udp(app);
    tcp_init(app);
	
    // run the loop
    uv_run(app->loop, UV_RUN_DEFAULT); // 8. Run the Event Loop
}



