#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include "../client.h"
#include "../view/rect.h"
#include "../view/text.h"
#include "../view/circle.h"
#include "../view/line.h"
// #include "../view/view.h"
#include "../net/udp.h"

#define BLOCK_W 25
#define BLOCK_H 150



static SDL_FRect username_left = (SDL_FRect){
	.x = 10,
	.y = TEXTBOX_HEIGHT + 10,
	.w = TEXTBOX_WIDTH,
	.h = TEXTBOX_HEIGHT,
};


static SDL_FRect username_right = (SDL_FRect){
	.x = WINDOW_W - 50,
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
	.x = WINDOW_W - 50,
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
	circle->obj.speed = 20;
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

static void init_angle_indicator(App* app) {
	Circle* circle = &(app->gameplay.objects.circle);
	line line;
	line.originx = circle->obj.pos.x + circle->radius +  10;
	line.originy = circle->obj.pos.y;
	line.tailx = line.originx + cos(circle->obj.direction) * 100;
	line.taily = line.originy + sin(circle->obj.direction) * 100;
	app->gameplay.objects.line = line;
}


static void init(App* app) {
	// set initial state to pointing
	app->gameplay.state = pointing;
	init_blocks(app);
	init_circle(app);
	init_usernames(app);
	init_angle_indicator(app);
}

static void angle_indicator_up(App* app) {
	app->gameplay.objects.circle.obj.direction -= M_PI * 0.025;
	init_angle_indicator(app);
}


static void angle_indicator_down(App* app) {
	app->gameplay.objects.circle.obj.direction += M_PI * 0.025;
	init_angle_indicator(app);
}

static void shoot(App* app) {
	app->gameplay.state = playing;
}


static void handle_input_pointing(App* app, SDL_Event* event) {
	switch (event->key.key) {
		case SDLK_UP:
			angle_indicator_up(app);		
			break;
		case SDLK_DOWN:
			angle_indicator_down(app);		
			break;
		case SDLK_SPACE:
			printf("handling return\n");
			shoot(app);
			break;
	}
}

static void block_left_up(App* app) {
	SDL_FRect* block = &(app->gameplay.objects.block_left);
	// check that block is in bounds
	if (block->y > 0) {
		block->y -= 10;
	} 


}

static void block_left_down(App* app) {
	SDL_FRect* block = &(app->gameplay.objects.block_left);
	// check that block is in bounds
	if (block->y < WINDOW_H) {
		block->y += 10;
	}
	
	char* block_position = calloc(7, sizeof(char));

	sprintf(block_position, "%f.0", block->y);

	udp_write_msg_1("block", block_position);

	// encode_message(char *type, char *content)
	// udp_write(
}

static void handle_input_playing(App* app, SDL_Event* event) {
	switch (event->key.key) {
		case SDLK_UP:
			block_left_up(app);
			break;
		case SDLK_DOWN:
			block_left_down(app);
			break;
	}
}


static void input(App* app, SDL_Event* event) {
	switch (app->gameplay.state) {
		case pointing:
			handle_input_pointing(app, event);
			break;
		case playing:
			handle_input_playing(app, event);
			break;
		case gameover:
			break;
			
	}
}


void handle_collisions(App *app) {

		gameplay_objects* objs = &(app->gameplay.objects);
		
		Circle* circle = &(objs->circle);
		float circlex = circle->obj.pos.x;
		float circley = circle->obj.pos.y;
		float rect_right_x = objs->block_right.x;
		float rect_right_y = objs->block_right.y;
		float rect_left_x = objs->block_left.x;
		float rect_left_y = objs->block_left.y;
		float rect_left_h = objs->block_left.h;
		float rect_right_h = objs->block_right.h;


		if (circlex + circle->radius >= rect_right_x && 
		    circley - circle->radius <= rect_right_y + rect_right_h &&
		    circley + circle->radius >= rect_right_y
		) {
			circle_bounce_wall_left_right(circle);
		} else if (circlex - circle->radius <= rect_left_x && 
		    circley - circle->radius <= rect_left_y + rect_left_h &&
		    circley + circle->radius >= rect_left_y
		) {
			circle_bounce_wall_left_right(circle);
		}

}




static void render(App* app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	render_usernames(app);
	
	if (app->gameplay.state == playing) { 
		handle_collisions(app);
		move_circle(&(app->gameplay.objects.circle));
	}
	render_circle(renderer, &(app->gameplay.objects.circle));
	render_blocks(app);
	switch (app->gameplay.state) {
		case pointing:
			render_line(app->renderer, &(app->gameplay.objects.line));
			break;
		default:
			break;
	}
}





extern fr_handler h_gameplay = (fr_handler){
	.init = (init_handler*)init,
	.input = (input_handler*)input,
	.render = (render_handler*)render,
};




