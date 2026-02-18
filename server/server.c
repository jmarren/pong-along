
#include "server.h"
#include "uv.h"
#include "udp.h"
#include "tcp.h"
#include "models/users.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../dsa/array.h"


server_t server;

bool is_emory(user_t* user) {

	if (strcmp("emory", user->username) == 0) {
		return true;
	}
	return false;
}



// DEFINE_ARR_TYPE(my_struct, int)
// DEFINE_ARR_CREATE(my_struct, int)

// Callbacks (on_new_connection, on_read, on_write, on_alloc, etc.) 
// must be defined to handle specific events.
int main(void) {

	int_arr arr = create_int_arr();

	append_int_arr(&arr, 1);
	
	printf("%d\n", arr.base[0]);


    // array arr = array_create();
    //
    // array_append(&arr, (void*)1);
    // array_append(&arr, (void*)1);



    // int_arr arr = create_int_arr();
    // append_int_arr(&arr, 1);

    // printf("arr->base[] = %d\n", arr.base[0]);
	





    // DECLARE_VAR(my_struct, my_int);
    //
    // my_int.id = 3;
    //
    // printf("%d\n", my_int.id);


    // array_append(&arr, (void*)2);
    // array_append(&arr, (void*)3);
    // array_append(&arr, (void*)4);
    // array_append(&arr, (void*)5);
    // array_append(&arr, (void*)6);
    // array_append(&arr, (void*)7);
    // array_append(&arr, (void*)8);
    // array_append(&arr, (void*)9);
    // array_append(&arr, (void*)10);
    // array_append(&arr, (void*)11);

    // printf("%d\n", (int)(intptr_t)arr.base[0]);
    // printf("%d\n", (int)(intptr_t)arr.base[1]);
    // printf("%d\n", (int)(intptr_t)arr.base[arr.len - 1]);

    // users_arr_init(&(server.active_users), 10,  100);
    //
    //
    // user_t user1;
    // user_t user2;
    // user1.username = "gabe";
    // user2.username = "emory";
    //
    // array_append(&arr, (void*)&user1);
    //
    // printf("%s\n", ((user_t*)arr.base[arr.len - 1])->username);
    //
    // users_arr_append(&(server.active_users), &user1);
    // users_arr_append(&(server.active_users), &user2);
    //
    // fflush(stdout);
    //
    // users_arr_print_usernames(&(server.active_users));
    //
    // users_arr filtered_arr = users_arr_filter(&(server.active_users), is_emory);
    //
    // users_arr_print_usernames(&filtered_arr);
    //
    // create default libuv loop
    server.loop = uv_default_loop();

    server_init_tcp(&server);
    server_init_udp(&server);

    // run the loop with default flag
    uv_run(server.loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

