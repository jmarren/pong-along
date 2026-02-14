


#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_timer.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../view/view.h"
#include "../view/circle.h"
#include "../net/net.h"

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

int handle_keydown(App* app, SDL_Event* event) {

			SDL_Keycode key = event->key.key;
			Circle* circle = &(app->circle);

			switch (key) {
				case SDLK_ESCAPE :
					return QUIT;
					break;
				case SDLK_SPACE:
					handle_space(app);
					break;
				case SDLK_UP:
					handle_up(app);
					break;
				case SDLK_DOWN:
					handle_down(app);
					break;
			}

			
			if (app->game_phase == pointing) {
				switch (key) {
					case SDLK_N:
						circle->obj.direction += M_PI * 0.05;
						break;
					case SDLK_P:
						circle->obj.direction -= M_PI * 0.05;
						break;

				}
			}


			if (app->game_phase == typing) {
				switch (key) {
					case SDLK_BACKSPACE:
						size_t len;
						len = strlen(app->text_input);
						if (len > 0) {
							app->text_input[len - 1] = '\0';
						}

						break;
					case SDLK_RETURN:
						SDL_StopTextInput(app->window);
						// strncpy(req_msg, 
						// strcat(char *restrict dest, const char *restrict src)
						strncpy(app->username, app->text_input, 100);
						app->game_phase = pointing;
						char* req_msg = malloc(strlen("username: ") + sizeof(app->username));
						strncpy(req_msg, "username: ", strlen("username: "));
						strcat(req_msg, app->username);
						net_write(req_msg);
						break;
				}
			}

			return CONTINUE;
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
			printf("hit rect_right\n");
			circle_bounce_wall_left_right(app);
		} else if (circlex - circle->radius <= rect_left_x && 
		    circley - circle->radius <= rect_left_y + app->rect_left.h &&
		    circley + circle->radius >= rect_left_y
		) {
			printf("hit rect_right\n");
			circle_bounce_wall_left_right(app);
		}

}





void loop_start(App* app) {

	input_text = malloc(sizeof(char) * 100);
	
	int ticks = SDL_GetTicks();


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


			// if (key == SDLK_W) {
			// 	net_write("hi\r\n");			
			// }
			// if (key == SDLK_K) {
			// 	char* msg = "bye\r\n";
			// 	printf("msg = %s\n", msg);
			// 	net_write(msg);			
			// }
