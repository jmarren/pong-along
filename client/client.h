
#ifndef CLIENT_H
#define CLIENT_H


#include "uv.h"
#include "../vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "view/physics.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include "../dsa/string.h"
#include "../shared/macro.h"



typedef struct {
	Object obj;
	int radius;
} Circle;


typedef enum {
	initializing,
	enter_username,
	select_opponent,
	gameplay,
} frame;


typedef struct  {
	SDL_FRect rect;
	char* text;
} text_component;


typedef struct {
	text_component input_component;
	text_component title_component;
} fr_enter_username;


typedef struct { 
	int len;
	int selected_index;
	SDL_FRect selected_container;
	text_component base[MAX_ACTIVE_USERS];
} player_component_list;

typedef struct {
	text_component title_component;
	player_component_list player_components;
} fr_select_opponent;

typedef struct {
	string_arr possible_opponents;
	int selected_opponent;
	SDL_FRect rect_left;
	SDL_FRect rect_right;
	Circle circle;
} fr_pointing;

typedef struct {
	string opponent_username;
} fr_playing;


typedef struct { 
	fr_enter_username enter_username;
	fr_select_opponent select_opponent;
	fr_pointing pointing;
	fr_playing playing;
} frames_t;


typedef enum {
	pointing,
	playing,
	gameover,
} gameplay_state;



typedef struct {
	float originx;
	float originy;
	float tailx;
	float taily;
} line;


typedef struct {
	Circle circle;
	SDL_FRect block_left;
	SDL_FRect block_right;
	line line;
} gameplay_objects;

typedef struct {
	gameplay_state state;
	text_component username_left;
	text_component username_right;
	gameplay_objects objects;
} gameplay_data;


	// float originx = app->circle.obj.pos.x + app->circle.radius +  10;
	// float originy = app->circle.obj.pos.y;
	// float tailx = originx + cos(app->circle.obj.direction) * 100;
	// float taily = originy + sin(app->circle.obj.direction) * 100;
	// SDL_RenderLine(app->renderer, originx, originy, tailx, taily);

	//
	// SDL_FRect rect_left;
	// SDL_FRect rect_right;
	// SDL_FRect dashboard_textbox;
	// SDL_FRect dashboard_title;
	// SDL_FRect gameplay_header;
	// SDL_FRect* player_list;
	// Circle circle;
	// char* username;
	// char* text_input;
	// SDL_FRect text_input_rect;
	// CharpList active_users;
	// int selected_opponent;

typedef void (init_handler)(void* app);
typedef void (input_handler)(void* app, SDL_Event* event);
typedef void (render_handler)(void* app);
typedef struct {
	init_handler* init;
	input_handler* input;
	render_handler* render;
} fr_handler;



typedef struct {
	// always present
	uv_loop_t* loop;
	SDL_Renderer* renderer;
	SDL_Window* window;
	TTF_Font * font;
	Uint32 read_event_type;
	frame current_frame;
	frames_t frames;
	char username[MAX_USERNAME_CHARS];
	char opponent_username[MAX_USERNAME_CHARS];
	fr_handler handlers[5];
	gameplay_data gameplay;
} App;



#endif
