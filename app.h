#ifndef MAIN_H
#define MAIN_H


#include "uv.h"
#include "vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "view/physics.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

typedef struct {
	Object obj;
	int radius;
} Circle;


typedef enum {
	initializing,
	typing,
	playing,
	pointing,
	choosing_opponent,
} game_phase;

typedef struct {
	char** base;
	int len;
	size_t capacity;
} CharpList; 



typedef struct {
	char* username;
	uv_stream_t* stream;
} user;

typedef struct {
	char* type;
	char* content;
} message;

typedef struct {
	user* users;
	int count;
} users_list;


typedef struct {
	char** messages;
	int count;
} raw_msg_list;

typedef struct {
	message* base;
	int len;
} message_list;

typedef struct {
	SDL_Renderer* renderer;
	SDL_FRect rect_left;
	SDL_FRect rect_right;
	SDL_FRect dashboard_textbox;
	SDL_FRect dashboard_title;
	SDL_FRect gameplay_header;
	SDL_FRect* player_list;
	Uint32 read_event_type;
	Circle circle;
	char* username;
	SDL_Window* window;
	game_phase game_phase;
	// bool game_started;
	TTF_Font * font;
	char* text_input;
	SDL_FRect text_input_rect;
	CharpList active_users;
	int selected_opponent;
} App;

#endif
