


#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <stdio.h>
#include "../net/net.h"
#include "../view/view.h"
 

void loop_start(void) {
	

	// view_circle_move(renderer, 10, 10);
	// printf("tick\n");
	
	int ticks = SDL_GetTicks();


	while (true) {

	 	SDL_Event event;
		
		int new_ticks = SDL_GetTicks();

		if (new_ticks - ticks > 20) {
		   view_circle_move();
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
	
			if (event.key.key == SDLK_UP) {	
				view_handle_up();		
			}
			
			if (event.key.key == SDLK_DOWN) {	
				view_handle_down();
			}
			
			if (event.key.key == SDLK_H) {
				view_handle_h();
			}

			if (key == SDLK_W) {
				net_write("hi\r\n");			
			}
			if (key == SDLK_K) {
				char* msg = "bye\r\n";
				printf("msg = %s\n", msg);
				net_write(msg);			
			}
		}

			view_render();

		}
	}

	end:

	

}
