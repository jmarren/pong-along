


#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_timer.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "../net/net.h"
#include "../view/view.h"
 


void loop_start(App* app) {
	

	// view_circle_move(renderer, 10, 10);
	// printf("tick\n");
	
	int ticks = SDL_GetTicks();


	while (true) {

	 	SDL_Event event;
		
		int new_ticks = SDL_GetTicks();

		if (new_ticks - ticks > 20) {
			


		   ticks = new_ticks;
	
		    while (SDL_PollEvent(&event)) {  
			
			if (event.type == net_read_evt.type) {
				printf("uv event fired\n");
			}
		

			if (event.type == SDL_EVENT_QUIT){
				goto end;
			}

			SDL_Keycode key = event.key.key;

			if (key == SDLK_ESCAPE) {
				goto end;
			}

			if (key == SDLK_SPACE) {
				app->game_started = true;
				app->circle->obj.speed = 20;
			}
	
			if (key == SDLK_UP) {	
				view_handle_up();		
			}
			
			if (key == SDLK_DOWN) {	
				view_handle_down();
			}
			
			if (key == SDLK_H) {
				view_handle_h();
			}

			if (key == SDLK_N) {
				app->circle->obj.direction += M_PI * 0.05;
			} 
			if (key == SDLK_P) {
				app->circle->obj.direction -= M_PI * 0.05;
			} 

			// if (key == SDLK_W) {
			// 	net_write("hi\r\n");			
			// }
			// if (key == SDLK_K) {
			// 	char* msg = "bye\r\n";
			// 	printf("msg = %s\n", msg);
			// 	net_write(msg);			
			// }
		    }


		   if (app->game_started) {
			view_circle_move();
		   } else {
			app->circle->obj.pos.y = app->rect_left->y + app->rect_left->h / 2;
			app->circle->obj.pos.x = app->rect_left->x + app->circle->radius + 10;
		   }

		   view_render();

		}
	}

	end:

	

}
