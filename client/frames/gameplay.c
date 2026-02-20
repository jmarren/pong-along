#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "../client.h"
#include "../view/rect.h"
#include "../view/text.h"
#include "../view/circle.h"

#define BLOCK_W 25
#define BLOCK_H 150



static SDL_FRect username_left = (SDL_FRect){
	.x = 10,
	.y = TEXTBOX_HEIGHT + 10,
	.w = TEXTBOX_WIDTH,
	.h = TEXTBOX_HEIGHT,
};


static SDL_FRect username_right = (SDL_FRect){
	.x = 500,
	.y = TEXTBOX_HEIGHT + 10,
	.w = TEXTBOX_WIDTH,
	.h = TEXTBOX_HEIGHT,
};

static SDL_FRect block_left = (SDL_FRect){
	.x = FR_MARGIN_LEFT,
	.y = 100,
	.w = BLOCK_W,
	.h = BLOCK_H,
};


static SDL_FRect block_right = (SDL_FRect){
	.x = FR_MARGIN_LEFT + 1000,
	.y = 100,
	.w = BLOCK_W,
	.h = BLOCK_H,
};



static void init_circle(App* app) {
	Circle* circle = &(app->gameplay.objects.circle);
	circle->radius = 25;
	circle->obj.pos.x = app->gameplay.objects.block_left.x + BLOCK_W + 10 + circle->radius;
	circle->obj.pos.y = app->gameplay.objects.block_left.y + (BLOCK_H / 2.0);
	circle->obj.direction = 0.21 * M_PI;
	circle->obj.speed = 0;
};

static void init_usernames(App* app) {
	text_component* rect_left = &(app->gameplay.username_left);
	text_component* rect_right = &(app->gameplay.username_right);

	// set text of each to point to corresponding usernames
	rect_left->text = app->username;
	rect_right->text = app->opponent_username;

	// set each rect to corresponding rect
	rect_left->rect = username_left;
	rect_right->rect = username_right;
}

static void init_blocks(App* app) {
	app->gameplay.objects.block_left = block_left;
	app->gameplay.objects.block_right = block_right;
}

static void render_blocks(App* app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_RenderRect(renderer, &(app->gameplay.objects.block_left));
	SDL_RenderRect(renderer, &(app->gameplay.objects.block_right));

}


static void render_usernames(App* app) {
	text_component* username_left = &(app->gameplay.username_left);
	text_component* username_right = &(app->gameplay.username_right);
	text_render(app, username_left);
	text_render(app, username_right);
}



static void init(App* app) {
	// set initial state to pointing
	app->gameplay.state = pointing;
	init_blocks(app);
	init_circle(app);
	init_usernames(app);
}


static void input(App* app, SDL_Event* event) {
}



static void render(App* app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	render_usernames(app);
	render_circle(renderer, &(app->gameplay.objects.circle));
	render_blocks(app);
}





extern fr_handler h_gameplay = (fr_handler){
	.init = (init_handler*)init,
	.input = (input_handler*)input,
	.render = (render_handler*)render,
};




