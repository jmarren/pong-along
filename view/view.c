#include "view.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <math.h>
#include "circle.h"



int init_renderer(App* app) {
    	if (!SDL_CreateWindowAndRenderer("Hello World", WINDOW_W, WINDOW_H, SDL_WINDOW_FULLSCREEN, &(app->window), &(app->renderer))) {
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
    	}

	return 0;
}


int init_rects(App* app) {
	
	SDL_FRect* rect_left = &(app->rect_left);
	SDL_FRect* rect_right = &(app->rect_right);

	// init rect_left
	rect_left->x = 100;
	rect_left->y = 100;
	rect_left->w = 10;
	rect_left->h = 100;

	// init rect_right
	rect_right->x = 1000;
	rect_right->y = 100;
	rect_right->w = 10;
	rect_right->h = 100;

	return 0;
}







void view_init(App* app) {
	SDL_InitSubSystem(0);
	init_renderer(app);
	init_rects(app);
	circle_init(app);
}

void draw_window_borders(App* app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_RenderLine(renderer, 0, 0, 0, WINDOW_H);
	SDL_RenderLine(renderer, WINDOW_W, 0, WINDOW_W, WINDOW_H);
	SDL_RenderLine(renderer, 0, 0, WINDOW_W, 0);
	SDL_RenderLine(renderer, 0, WINDOW_H, WINDOW_W, WINDOW_H);
}

void render_angle_indicator(App* app) {
	float originx = app->circle.obj.pos.x + app->circle.radius +  10;
	float originy = app->circle.obj.pos.y;
	float tailx = originx + cos(app->circle.obj.direction) * 100;
	float taily = originy + sin(app->circle.obj.direction) * 100;
	SDL_RenderLine(app->renderer, originx, originy, tailx, taily);
}



void view_render(App *app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &(app->rect_left));
	SDL_RenderFillRect(renderer, &(app->rect_right));
	draw_window_borders(app);
	circle_render(app);
	
	if (!app->game_started) {
		render_angle_indicator(app);
	}

	SDL_RenderPresent(renderer);
}



// void view_handle_up(void) {
// 	rect_left.y -= 36;
// 	char buffer[40];
// 	snprintf(buffer, sizeof(buffer), "rect-left=%.lf\r\n", rect_left.y);
// 	net_write((char *)&buffer);
// }
//
//
// void view_handle_down(void) {
// 	rect_left.y += 36;
// 	char buffer[40];
// 	snprintf(buffer, sizeof(buffer), "rect-left=%.lf\r\n", rect_left.y);
// 	net_write((char *)&buffer);
// }

// void view_handle_h(void) {
// 	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
// 	SDL_RenderClear(renderer);
// 	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// 	SDL_RenderFillRect(renderer, &rect_left);
// 	SDL_RenderPresent(renderer);
// }

// void view_hello_world(void) {
//    	const char *message = "Hello World!";
//    	int w = 0, h = 0;
//    	float x, y;
//    	const float scale = 1.0f;
//
//    	/* Center the message and scale it up */
//    	SDL_GetRenderOutputSize(renderer, &w, &h);
//    	// SDL_SetRenderScale(renderer, scale, scale);
//    	x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
//    	y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;
//
//    	/* Draw the message */
//    	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    	SDL_RenderClear(renderer);
//    	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    	SDL_RenderDebugText(renderer, x, y, message);
//    	SDL_RenderPresent(renderer);
//
// }




