#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <stdbool.h>
#include <stddef.h>
#include "client.h"
#include "frames/enter_username.h"
#include "frames/select_opponent.h"

#define QUIT 1
#define CONTINUE 0



// handle global events every iteration
int handle_global_events(App* app, SDL_Event* event) {
	
	if (event->type == SDL_EVENT_QUIT) {
		return QUIT;
	}
	
	if (event->key.key == SDLK_ESCAPE) {
		return QUIT;
	}
	return CONTINUE;
}

// handle events (global and frame-specific)
int handle_events(App* app) {
	
	 SDL_Event event;
	 while (SDL_PollEvent(&event)) {  
		// handle global events and quit if neccessary
		if (handle_global_events(app, &event) == QUIT) {
			return QUIT;
		};

		// use the current frame input handler
		app->handlers[app->current_frame].input(app, &event);
	 }

	return CONTINUE;
}

void render(App* app) {
		app->handlers[app->current_frame].render(app);
		SDL_RenderPresent(app->renderer);
}



void loop_start(App* app) {

	enter_username_init(app);

	int ticks = SDL_GetTicks();

	while (true) {
		
		int new_ticks = SDL_GetTicks();

		if (new_ticks - ticks > 10) {
		    // set ticks to new_ticks
		    ticks = new_ticks;

		    // handle global events and quit if neccessary
		    if (handle_events(app) == QUIT) break;
			
		    // render the app
		    render(app);
		}
	}
}




// void handle_space(App* app) {
// 	app->game_phase = playing;
// 	app->circle.obj.speed = 20;
// }
//
// void handle_up(App* app) {
// 	app->rect_left.y -= 36;
// }
//
// void handle_down(App* app) {
// 	app->rect_left.y += 36;
// }
//
// void handle_return(App* app) {
// 	// stop taking input
// 	SDL_StopTextInput(app->window);
//
// 	// copy text_input to username
// 	strncpy(app->username, app->text_input, 100);
//
// 	// set game phase to pointing
// 	app->game_phase = choosing_opponent;
//
// 	tcp_write_msg_1("username", app->username);
//
// 	tcp_write_msg_1("query", "players");
//
// }
//
// void handle_backspace(App* app) {
// 	// string_truncate(app->)
// 	// size_t len;
// 	// len = strlen(app->text_input);
// 	// if (len > 0) {
// 	// 	app->text_input[len - 1] = '\0';
// 	// }
// }
//
//
// void select_opponent(App* app) {
// 	tcp_write_msg_1("selected-opponent", app->active_users.base[app->selected_opponent]);
// }
//
//
// int handle_keydown(App* app, SDL_Event* event) {
//
// 	SDL_Keycode key = event->key.key;
// 	Circle* circle = &(app->circle);
//
// 	switch (key) {
// 		case SDLK_ESCAPE :
// 			return QUIT;
// 			break;
// 	}
//
// 	if (app->game_phase == choosing_opponent) {
// 		switch(key) {
// 			case SDLK_UP:
// 				if (app->selected_opponent > 0) {
// 					app->selected_opponent--;
// 				}
// 				break;
// 			case SDLK_DOWN:
// 				if (app->selected_opponent < app->active_users.len - 1) {
// 					app->selected_opponent++;
// 				}
// 				break;
// 			case SDLK_RETURN:
// 				select_opponent(app);
// 				break;
//
// 		}
// 	}
//
//
// 	if (app->game_phase == pointing) {
// 		switch (key) {
// 			case SDLK_N:
// 				circle->obj.direction += M_PI * 0.05;
// 				break;
// 			case SDLK_P:
// 				circle->obj.direction -= M_PI * 0.05;
// 				break;
// 			case SDLK_SPACE:
// 				handle_space(app);
// 				break;
//
// 		}
// 	}
//
//
// 	if (app->game_phase == typing) {
// 		switch (key) {
// 			case SDLK_BACKSPACE:
// 				handle_backspace(app);
// 				break;
// 			case SDLK_RETURN:
// 				handle_return(app);
// 				break;
// 		}
//
// 	}
//
// 	if (app->game_phase == playing) {
// 		switch (key) {
// 			case SDLK_UP:
// 				handle_up(app);
// 				udp_write("up");
// 				break;
// 			case SDLK_DOWN:
// 				handle_down(app);
// 				udp_write("down");
// 				break;
// 		}
// 	}
//
// 	return CONTINUE;
// }
//
//
// void print_active_users(App* app) {
// 	printf("active_users: \n");
// 	for (int i = 0; i < app->active_users.len; i++) {
// 		printf("- %s\n", app->active_users.base[i]);
// 	}
//
// }
//
// void handle_message(App* app, message* msg) {
//
// 	if (strcmp(msg->type, "players") == 0) {
// 		const char s = ','; // Delimiters: space and comma
// 		char *token;
//
// 		// Get the first token
// 		// The first call to strtok() uses the original string
// 		token = strtok(msg->content, &s);
//
// 		int i = 0;
// 		// Walk through other tokens
// 		while (token != NULL) {
// 			strlen(token) > 0 ? app->active_users.base[i] = token : NULL;
// 			app->active_users.len = i + 1;
// 			i++;
//
// 			token = strtok(NULL, &s);
// 		}
//
// 		print_active_users(app);
// 		init_player_list(app);
// 	}
//
//
// 	if (strcmp(msg->type, "match") == 0) {
// 		app->game_phase = pointing;
// 	}
// }
//
//
//
// void handle_read_event(App* app, SDL_UserEvent* evt) {
// 	char* data1 = (char*)evt->data1;
//
// 	raw_msg_list raw_msgs;
// 	raw_msgs = parse_messages(data1);
// 	print_raw_messages(&raw_msgs);
// 	message_list msgs = parse_raw_message_list(&raw_msgs);
//
// 	for (int i = 0; i < msgs.len; i++) {
// 		handle_message(app, &(msgs.base[i]));
// 	}
// }
//
//
//
//
//
//
// void handle_collisions(App *app) {
//
// 		Circle* circle = &(app->circle);
// 		float circlex = circle->obj.pos.x;
// 		float circley = circle->obj.pos.y;
// 		float rect_right_x = app->rect_right.x;
// 		float rect_right_y = app->rect_right.y;
// 		float rect_left_x = app->rect_left.x;
// 		float rect_left_y = app->rect_left.y;
//
//
// 		if (circlex + circle->radius >= rect_right_x && 
// 		    circley - circle->radius <= rect_right_y + app->rect_right.h &&
// 		    circley + circle->radius >= rect_right_y
// 		) {
// 			circle_bounce_wall_left_right(app);
// 		} else if (circlex - circle->radius <= rect_left_x && 
// 		    circley - circle->radius <= rect_left_y + app->rect_left.h &&
// 		    circley + circle->radius >= rect_left_y
// 		) {
// 			circle_bounce_wall_left_right(app);
// 		}
//
// }
//

// int handle_events(App* app) {
//
//
// 	 while (SDL_PollEvent(&event)) {  
// 		 switch (event.type) {
// 			 case SDL_EVENT_QUIT:
// 				return QUIT;
// 				break;
// 			 case SDL_EVENT_KEY_DOWN:
// 				if (handle_keydown(app, &event) == QUIT) {
// 					return QUIT;
// 				}
// 				break;
//
// 			case SDL_EVENT_TEXT_INPUT:
// 			    if (strlen(app->text_input) < 99) {
// 				app->text_input = strncat(app->text_input, event.text.text, 1);
// 			    }
// 			    break;
// 		 }
//
// 		if (event.type == app->read_event_type) {
// 			SDL_UserEvent* evt = (SDL_UserEvent*)&event;
// 			handle_read_event(app, evt);
// 		}
// 	 }
//
// 	return CONTINUE;
// }
//

		
			//   // handle events and quit if QUIT is returned
			//   if (handle_events(app) == QUIT) break;
			//
			//   if (app->game_phase == playing) {
			//    handle_collisions(app);
			//    circle_move(app);
			//   } else {
			// app->circle.obj.pos.y = app->rect_left.y + app->rect_left.h / 2;
			// app->circle.obj.pos.x = app->rect_left.x + app->circle.radius + 10;
			//   }

		   // view_render(app);

