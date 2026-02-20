#include "../client.h"
#include <SDL3/SDL_events.h>

static void init(App* app) {

}

static void input(App* app, SDL_Event* event) {

}

static void render(App* app) { 
	SDL_Renderer* renderer = app->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

extern fr_handler h_pointing = (fr_handler){
	.init = (init_handler*)init,
	.input = (input_handler*)input,
	.render = (render_handler*)render,
};

