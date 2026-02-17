
#include "server.h"
#include "uv.h"
#include "udp.h"
#include "tcp.h"
#include "models/users.h"
#include <stdbool.h>
#include <string.h>


server_t server;

bool is_emory(user_t* user) {

	if (strcmp("emory", user->username) == 0) {
		return true;
	}
	return false;
}


// Callbacks (on_new_connection, on_read, on_write, on_alloc, etc.) 
// must be defined to handle specific events.
int main(void) {

    users_arr_init(&(server.active_users), 10,  100);


    user_t user1;
    user_t user2;
    user1.username = "gabe";
    user2.username = "emory";

    users_arr_append(&(server.active_users), &user1);
    users_arr_append(&(server.active_users), &user2);

    fflush(stdout);

    users_arr_print_usernames(&(server.active_users));

    users_arr filtered_arr = users_arr_filter(&(server.active_users), is_emory);

    users_arr_print_usernames(&filtered_arr);

    // create default libuv loop
    server.loop = uv_default_loop();

    server_init_tcp(&server);
    server_init_udp(&server);

    // run the loop with default flag
    uv_run(server.loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

