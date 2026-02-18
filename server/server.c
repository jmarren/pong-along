
#include "server.h"
#include "models/users.h"
#include "uv.h"
#include "udp.h"
#include "tcp.h"
#include <stdbool.h>
// #include "models/users.h"


server_t server;


void user_print_username(user_t user) {
	printf("username = %s\n", user.username);
}

bool name_is_anthony(user_t user) {
	if (user.username == "anthony") return true;
	return false;
}

int main(void) {
	
    user_arr users = create_user_arr();
	
    user_t anthony;
    user_t james;

    anthony.username = "anthony";
    james.username = "james";

    append_user_arr(&users, &anthony);
    append_user_arr(&users, &anthony);
    append_user_arr(&users, &james);

    printf("users.base[0].username = %s\n", users.base[0].username);

    for_each_user_arr(&users, &user_print_username);

    user_arr filtered_arr = filter_user_arr(&users, name_is_anthony);
	
    printf("anthony only: \n");
    for_each_user_arr(&filtered_arr, &user_print_username);

    // filter_user_arr(user_arr *arr, bool_from_user_t *fn)

    // server.active_users = create_user_arr();

    // create default libuv loop
    server.loop = uv_default_loop();
	

    server_init_tcp(&server);
    server_init_udp(&server);

    // run the loop with default flag
    uv_run(server.loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

