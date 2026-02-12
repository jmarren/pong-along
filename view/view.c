#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <stdio.h>
#include "../net/net.h"
#include "circle.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_FRect rect_left;
SDL_FRect rect_right;


int init_renderer(void) {
    	if (!SDL_CreateWindowAndRenderer("Hello World", 1000, 100, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
    	}

	return 0;
}


int init_rects(void) {
	// init rect_left
	rect_left.x = 100;
	rect_left.y = 100;
	rect_left.w = 10;
	rect_left.h = 100;

	// init rect_right
	rect_right.x = 1000;
	rect_right.y = 100;
	rect_right.w = 10;
	rect_right.h = 100;

	return 0;
}




void view_circle_move(void) { 
	circle_move(renderer);
}





// Uint32 timer_callback(void *userdata, SDL_TimerID timerID, Uint32 interval) {
// 	// SDL_PushEvent(&net_read_evt);
// 	printf("tick\n");
// 	return interval;
// }


void view_init(void) {
	SDL_InitSubSystem(0);
	init_renderer();
	init_rects();
	circle_init();
}

void view_render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderLine(renderer, 100, 100, 300, 300);
	SDL_RenderFillRect(renderer, &rect_left);
	SDL_RenderFillRect(renderer, &rect_right);
	circle_render(renderer);
	SDL_RenderPresent(renderer);

}



void view_handle_up(void) {

	rect_left.y -= 36;
	char buffer[40];
	snprintf(buffer, sizeof(buffer), "rect-left=%.lf\r\n", rect_left.y);
	net_write((char *)&buffer);
}


void view_handle_down(void) {
	rect_left.y += 36;
	char buffer[40];
	snprintf(buffer, sizeof(buffer), "rect-left=%.lf\r\n", rect_left.y);
	net_write((char *)&buffer);
}

void view_handle_h(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect_left);
	SDL_RenderPresent(renderer);
}

void view_hello_world(void) {
   	const char *message = "Hello World!";
   	int w = 0, h = 0;
   	float x, y;
   	const float scale = 1.0f;

   	/* Center the message and scale it up */
   	SDL_GetRenderOutputSize(renderer, &w, &h);
   	// SDL_SetRenderScale(renderer, scale, scale);
   	x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
   	y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

   	/* Draw the message */
   	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
   	SDL_RenderClear(renderer);
   	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
   	SDL_RenderDebugText(renderer, x, y, message);
   	SDL_RenderPresent(renderer);

}




