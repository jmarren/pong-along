

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <stdio.h>

#define PORT 7000
#define BACKLOG 1000

uv_tcp_t server;
uv_loop_t * loop;

typedef struct {
	char* username;
	uv_stream_t* stream;
} User;

typedef struct {
	char* type;
	char* content;
} Message;


User* users_online;

typedef struct {
	User* users;
	int count;
} UserList;

UserList active_users;

typedef struct {
	char** messages;
	int count;
} RawMessageList;


RawMessageList parse_messages(char* raw) {
	RawMessageList msgs;
	msgs.count = 0;
	msgs.messages = calloc(50, sizeof(char*));
	
	
	int* parse_indices = calloc(10, sizeof(int));
	int num_indices = 0;

	// get indices of /r/n delimiter
	for (int i = 0; i < (int)strlen(raw) - 1; i++) {
		if (raw[i] == '\r' && raw[i+1] == '\n') {
			parse_indices[num_indices] = i;
			num_indices++;
		} 
	}
	
	int start_index = 0;
	
	// copy contents into msgs
	for (int i = 0; i < num_indices; i++) {
		msgs.messages[msgs.count] = calloc(sizeof(char), parse_indices[i] - start_index);
		strncpy(msgs.messages[msgs.count], &raw[start_index], parse_indices[i] - start_index);
		printf("msgs.messages[msgs.count] = %s\n", msgs.messages[msgs.count]);

		start_index = parse_indices[i] + 2;
		msgs.count++;
	}


	return msgs;
}

			// // allocate space for message in list
			// msgs.messages[msgs.count] = malloc(strlen(curr_msg) + 1);
			//
			// // copy the current message to the 
			// strncpy(msgs.messages[msgs.count], &raw[i], strlen(curr_msg));
			// printf("found msg: %s\n", curr_msg);
			// i+=2;
			// curr_msg = "\0";
			// msgs.count++;


void trim_message(char* msg) {
	for (int i = 0; i < (int)strlen(msg); i++) {
		if (msg[i] == '\r') {
			msg[i] = '\0';
		}
	}
}

Message parse_message(char* msg) {
	Message message;
	
	int colon_index = 0;
	bool found = false;


	printf("parsing %s\n", msg);
 
	for (int i = 0; i < (int)strlen(msg); i++) {
		if (msg[i] == ':') {
			colon_index = i;
			found = true;
			break;
		} 
	}

	if (!found) {
		message.type = "unknown";
		message.content = msg;
		return message;
	}

	
	size_t type_len = colon_index * sizeof(char) + 1;
	size_t content_len = strlen(msg) - type_len + 1; 

	message.type = (char*)calloc(sizeof(char), type_len);
	message.content = (char*)calloc(sizeof(char), content_len);

	strncpy(message.type, msg, colon_index);
	strncpy(message.content, &msg[colon_index + 2], content_len - 1);
	
	return message;
}





void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void write_callback(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
	
    free(req);
}

void get_other_users(uv_stream_t* client, UserList* other_users) {
	
	other_users->count = 0;
	other_users->users = malloc(sizeof(User) * 10);

	// char** other_usernames = malloc(sizeof(char*) * 10);
	// int other_usernames_count = 0;

	for (int i = 0; i < active_users.count; i++) {
		User user = active_users.users[i];
		if (user.stream != client) {
			other_users->users[other_users->count] = user;
			other_users->count++;
		}
	}
}

void handle_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*) client, NULL);
        }
    } else if (nread > 0) {
	// trim the message to /r
	// trim_message(buf->base);


	printf("received = %s\n", buf->base);


	RawMessageList msg_list = parse_messages(buf->base);
		
	for (int i = 0; i < msg_list.count; i++) {
		Message msg = parse_message(msg_list.messages[i]);
		printf("msg %d = {\n %s \n %s\n}\n", i, msg.type, msg.content);

		if (strcmp(msg.type, "username") == 0) {
			printf("got new user request\n");
			// create a new user
			User user = {
				msg.content,
				client,
			};

			// add the user to active_users
			active_users.users[active_users.count] = user;
			active_users.count++;
		}

		if (strcmp(msg.content, "players?") == 0) {
			printf("players query\n");
			// UserList other_users;
			// get_other_users(client, &other_users);
			// printf("other users: %s\n", other_users.users->username);
				
			// printf("other_users.count: %d\n", other_users.count);
			
			printf("active_users.count: %d\n", active_users.count);

			char* res = calloc(100, sizeof(char));

			strncpy(res, "players: ", strlen("players: "));
			
			for (int i = 0; i < active_users.count; i++) {
				strcat(res, active_users.users[i].username);
				strcat(res, ", ");
			}
			
			// append delimiter
			strcat(res, "\r\n");
		
			printf("res: %s\n", res);


			// initialize the write buffer with the size of the message that was read (to echo back)
			uv_buf_t wrbuf = uv_buf_init(res, 100);
				
			
			// allocate the write request to write back
			uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));

			// write the message back to the client
			// with buf len of 1 and a write callback that 
			// frees the write request
			uv_write(req, client, &wrbuf, 1, write_callback);
		}
	}
	fflush(stdout);

	// // parse the message
	// Message message = parse_message(buf->base);
	//
	// // if message is a username add to active_users
	//
	// allocate the write request to write back
	// uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
	//
	// // initialize the write buffer with the size of the message that was read (to echo back)
	// uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
	//
	// // write the message back to the client
	// // with buf len of 1 and a write callback that 
	// // frees the write request
	//        uv_write(req, client, &wrbuf, 1, write_callback);
    } else {
	printf("read 0\n");
    }

    // free the client request buffer
    if (buf->base) {
        free(buf->base);
    }
}

void on_new_connection(uv_stream_t *server, int status) {
    // if error status, log error and return
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        return;
    }
	
    // allocate client 
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));

    // initialize tcp for client
    uv_tcp_init(loop, client);
	
    // accept connection and read if if no error is returned
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
	// pass the client (casted to uv_stream_t)
	// as well as a callback to allocate the buffer
	// and a function the handle the request
        uv_read_start((uv_stream_t*) client, alloc_buffer, handle_read);
    }
}




// Callbacks (on_new_connection, on_read, on_write, on_alloc, etc.) 
// must be defined to handle specific events.
int main(void) {

    active_users.users = malloc(sizeof(User) * 10);
    active_users.count = 0;


    // create default libuv loop
    loop = uv_default_loop();

    // initialize tcp server with loop and pointer to server
    uv_tcp_init(loop, &server);

    // create the socket address and assign it host & port 
    struct sockaddr_in addr;
    uv_ip4_addr("127.0.0.1", PORT, &addr);

    // bind the server to the socket address
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, UV_TCP_REUSEPORT); 

    // listen on the address with the on_new_connection callback
    int err = uv_listen((uv_stream_t*) &server, BACKLOG, on_new_connection);

    // handle listen error
    if (err) {
        fprintf(stderr, "Listen error: %s\n", uv_strerror(err));
        return 1;
    }

    // run the loop with default flag
    uv_run(loop, UV_RUN_DEFAULT); // 8. Run the Event Loop

    return 0;
}

