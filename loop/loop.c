#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_timer.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../view/view.h"
#include "../view/circle.h"
// #include "../net/net.h"
#include "../net/tcp.h"
#include "../net/udp.h"
#include "../net/parse.h"

#define QUIT 1
#define CONTINUE 0

char *input_text;
char *composition;
Sint32 cursor;
Sint32 selection_len;

void handle_space(App* app) {
	app->game_phase = playing;
	app->circle.obj.speed = 20;
}

void handle_up(App* app) {
	app->rect_left.y -= 36;
}

void handle_down(App* app) {
	app->rect_left.y += 36;
}

void handle_return(App* app) {
	// stop taking input
	SDL_StopTextInput(app->window);

	// copy text_input to username
	strncpy(app->username, app->text_input, 100);

	// set game phase to pointing
	app->game_phase = choosing_opponent;

	// calculate length of request message
	size_t req_msg_len = strlen("username: \r\n") + strlen(app->username) + 1;

	// calloc tcp request message
	char* req_msg = calloc(req_msg_len, sizeof(char));

	//  copy "username: " to req_message
	strncpy(req_msg, "username: ", strlen("username: "));

	// append the username to the end of req_msg
	strcat(req_msg, app->username);
	strcat(req_msg, "\r\n");

	// write the req_msg to the connection
	tcp_write(req_msg);
	// query player list
	tcp_write("players?\r\n");

}

void handle_backspace(App* app) {
	size_t len;
	len = strlen(app->text_input);
	if (len > 0) {
		app->text_input[len - 1] = '\0';
	}
}


void select_opponent(App* app) {
	char* write_req;
	
	char* opponent_username = app->active_users.base[app->selected_opponent];

	size_t len;
	len = strlen("selected-opponent: \r\n") + strlen(opponent_username);

	write_req = calloc(len + 1, sizeof(char));

	strncpy(write_req, "selected-opponent: ", strlen("selected-opponent: "));
	strncat(write_req, opponent_username, strlen(opponent_username));
	strncat(write_req, "\r\n", 3);
	printf("write_req = %s\n", write_req);

	tcp_write(write_req);
	
}


int handle_keydown(App* app, SDL_Event* event) {

	SDL_Keycode key = event->key.key;
	Circle* circle = &(app->circle);

	switch (key) {
		case SDLK_ESCAPE :
			return QUIT;
			break;
	}

	if (app->game_phase == choosing_opponent) {
		switch(key) {
			case SDLK_UP:
				if (app->selected_opponent > 0) {
					app->selected_opponent--;
				}
				break;
			case SDLK_DOWN:
				if (app->selected_opponent < app->active_users.len - 1) {
					app->selected_opponent++;
				}
				break;
			case SDLK_RETURN:
				select_opponent(app);
				break;

		}
	}

	
	if (app->game_phase == pointing) {
		switch (key) {
			case SDLK_N:
				circle->obj.direction += M_PI * 0.05;
				break;
			case SDLK_P:
				circle->obj.direction -= M_PI * 0.05;
				break;
			case SDLK_SPACE:
				handle_space(app);
				break;

		}
	}


	if (app->game_phase == typing) {
		switch (key) {
			case SDLK_BACKSPACE:
				handle_backspace(app);
				break;
			case SDLK_RETURN:
				handle_return(app);
				break;
		}

	}

	if (app->game_phase == playing) {
		switch (key) {
			case SDLK_UP:
				handle_up(app);
				udp_write("up");
				break;
			case SDLK_DOWN:
				handle_down(app);
				udp_write("down");
				break;
		}
	}

	return CONTINUE;
}


void print_active_users(App* app) {
	printf("active_users: \n");
	for (int i = 0; i < app->active_users.len; i++) {
		printf("- %s\n", app->active_users.base[i]);
	}

}

void handle_message(App* app, message* msg) {
		
	if (strcmp(msg->type, "players") == 0) {
		const char s = ','; // Delimiters: space and comma
		char *token;

		// Get the first token
		// The first call to strtok() uses the original string
		token = strtok(msg->content, &s);

		int i = 0;
		// Walk through other tokens
		while (token != NULL) {
			strlen(token) > 0 ? app->active_users.base[i] = token : NULL;
			app->active_users.len = i + 1;
			i++;

			token = strtok(NULL, &s);
		}

		print_active_users(app);
		init_player_list(app);
	}


	if (strcmp(msg->type, "match") == 0) {
		app->game_phase = pointing;
	}
}



void handle_read_event(App* app, SDL_UserEvent* evt) {
	char* data1 = (char*)evt->data1;
		
	raw_msg_list raw_msgs;
	raw_msgs = parse_messages(data1);
	print_raw_messages(&raw_msgs);
	message_list msgs = parse_raw_message_list(&raw_msgs);
			
	for (int i = 0; i < msgs.len; i++) {
		handle_message(app, &(msgs.base[i]));
	}
}



int handle_events(App* app) {

	SDL_Event event;

	 while (SDL_PollEvent(&event)) {  
		 switch (event.type) {
			 case SDL_EVENT_QUIT:
				return QUIT;
				break;
			 case SDL_EVENT_KEY_DOWN:
				if (handle_keydown(app, &event) == QUIT) {
					return QUIT;
				}
				break;

			case SDL_EVENT_TEXT_INPUT:
			    if (strlen(app->text_input) < 99) {
				app->text_input = strncat(app->text_input, event.text.text, 1);
			    }
			    break;
		 }

		if (event.type == app->read_event_type) {
			SDL_UserEvent* evt = (SDL_UserEvent*)&event;
			handle_read_event(app, evt);
		}
	 }

	return CONTINUE;
}




void handle_collisions(App *app) {

		Circle* circle = &(app->circle);
		float circlex = circle->obj.pos.x;
		float circley = circle->obj.pos.y;
		float rect_right_x = app->rect_right.x;
		float rect_right_y = app->rect_right.y;
		float rect_left_x = app->rect_left.x;
		float rect_left_y = app->rect_left.y;

		// printf("circlex = %f\ncircley = %f\nrect_right_x = %f\n rect_right_y = %f\n", circlex, circley, rect_right_x, rect_right_y);

		if (circlex + circle->radius >= rect_right_x && 
		    circley - circle->radius <= rect_right_y + app->rect_right.h &&
		    circley + circle->radius >= rect_right_y
		) {
			circle_bounce_wall_left_right(app);
		} else if (circlex - circle->radius <= rect_left_x && 
		    circley - circle->radius <= rect_left_y + app->rect_left.h &&
		    circley + circle->radius >= rect_left_y
		) {
			circle_bounce_wall_left_right(app);
		}

}





void loop_start(App* app) {

	input_text = malloc(sizeof(char) * 100);
	
	int ticks = SDL_GetTicks();

	//
	// write_udp("1");
	// write_udp("2");
	// write_udp("3");
	// write_udp("4");

	// write_udp("hi");


	while (true) {
		
		int new_ticks = SDL_GetTicks();

		if (new_ticks - ticks > 10) {
		   // set ticks to new_ticks
		   ticks = new_ticks;
		
		   // handle events and quit if QUIT is returned
		   if (handle_events(app) == QUIT) break;
			
		   if (app->game_phase == playing) {
			   handle_collisions(app);
			   circle_move(app);
		   } else {
			app->circle.obj.pos.y = app->rect_left.y + app->rect_left.h / 2;
			app->circle.obj.pos.x = app->rect_left.x + app->circle.radius + 10;
		   }

		   view_render(app);

		}
	}

}


