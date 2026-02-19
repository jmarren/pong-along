
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
	choosing_opponent,
} current_frame;


// typedef union string_or_charp 

typedef struct  {
	SDL_FRect rect;
	char* text;
} text_component;

typedef struct {
	text_component input_component;
	text_component title_component;
	// string text_input;
	// SDL_FRect text_input_rect;
	// SDL_FRect dashboard_textbox;
	// SDL_FRect dashboard_title;
} fr_enter_username;


typedef struct {
	string username;
	string_arr possible_opponents;
	int selected_opponent;
	SDL_FRect* player_list;
	SDL_FRect dashboard_textbox;
	SDL_FRect dashboard_title;
} fr_choosing_opponent;

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
	fr_choosing_opponent choosing;
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


typedef struct {
	// always present
	uv_loop_t* loop;
	SDL_Renderer* renderer;
	SDL_Window* window;
	TTF_Font * font;
	Uint32 read_event_type;
	current_frame current_frame;
	frames_t frames;
	char* username;
} App;

#endif
