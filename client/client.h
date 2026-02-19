
#ifndef CLIENT_H
#define CLIENT_H


#include "uv.h"
#include "../vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "view/physics.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "../dsa/string.h"
// #include "view/text.h"

typedef struct {
	Object obj;
	int radius;
} Circle;


typedef enum {
	initializing,
	enter_username,
	playing,
	pointing,
	select_opponent,
} frame;


	// typedef bool (bool_from_##x_Type)(x_Type)


// void enter_username_init(App* app);
// void enter_username_input(App* app, SDL_Event* event);
// void enter_username_render(App* app);

// typedef union string_or_charp 

typedef struct  {
	SDL_FRect rect;
	char* text;
} text_component;


typedef struct {
	text_component input_component;
	text_component title_component;
} fr_enter_username;


typedef struct {
	text_component title;
	text_component* players;
	int selected_index;
	// string username;
	// string_arr possible_opponents;
	// int selected_opponent;
	// SDL_FRect* player_list;
	// SDL_FRect dashboard_textbox;
	// SDL_FRect dashboard_title;
} fr_select_opponent;

typedef struct {
	string username;
	string_arr possible_opponents;
	int selected_opponent;
	SDL_FRect rect_left;
	SDL_FRect rect_right;
	Circle circle;
} fr_pointing;

typedef struct {
	string username;
	string opponent_username;
} fr_playing;


typedef struct { 
	fr_enter_username enter_username;
	fr_select_opponent select_opponent;
	fr_pointing pointing;
	fr_playing playing;
} frames_t;


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
	char* username;
	fr_handler handlers[5];
	
} App;



#endif
