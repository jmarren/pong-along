#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include "../net/net.h"

SDL_Event read_evt;

static int init_renderer(void);
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



void init_views(void) {
	// read_evt.type = SDL_RegisterEvents(1);
	// printf("set read_evt.type = %d\n", read_evt.type);
	//
	// init_net(read_evt);
	//
	SDL_InitSubSystem(0);

	init_renderer();
	init_rects();
	
}


int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;


    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int
SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;


    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


void handle_up(void) {

	rect_left.y -= 36;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderLine(renderer, 100, 100, 300, 300);
	SDL_RenderFillRect(renderer, &rect_left);
	SDL_RenderFillRect(renderer, &rect_right);
	SDL_RenderDrawCircle(renderer, 120, 120, 50);
	SDL_RenderFillCircle(renderer, 120, 120, 50);
	// SDL_RenderDebugText(renderer, x, y, "hi john");
	SDL_RenderPresent(renderer);
}


void handle_down(void) {
	rect_left.y += 36;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderLine(renderer, 100, 100, 300, 300);
	SDL_RenderFillRect(renderer, &rect_left);
	SDL_RenderFillRect(renderer, &rect_right);
	SDL_RenderDrawCircle(renderer, 120, 120, 50);
	SDL_RenderFillCircle(renderer, 120, 120, 50);
	// SDL_RenderDebugText(renderer, x, y, "hi john");
	SDL_RenderPresent(renderer);
}

void handle_h(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect_left);
	SDL_RenderPresent(renderer);
}

int hello_world(void) {
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




