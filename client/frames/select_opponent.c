#include "../client.h"
#include "../view/rect.h"
#include "../view/text.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include "../net/tcp.h"

static void render_players(App* app) {
	player_component_list* players = &(app->frames.select_opponent.player_components);

	int selected_index = players->selected_index;

	SDL_FRect* selected_container =  &(players->selected_container);
		
	for (int i = 0; i < players->len; i++) {
		text_render(app, &(players->base[i]));
		if (i == selected_index) { 
			selected_container->y = players->base[i].rect.y - 5;
			SDL_RenderRect(app->renderer, selected_container);
		}
	} 
} 

static void init(App* app) {
	text_component* title = &(app->frames.select_opponent.title_component);
	player_component_list* players = &(app->frames.select_opponent.player_components);

	players->selected_index = 0;
	players->len = 0;
	title->text = "select your opponent";
	title->rect = title_rect;

	players->selected_container.x = FR_MARGIN_LEFT;
	players->selected_container.w = TEXTBOX_WIDTH + 10;
	players->selected_container.h = TEXTBOX_HEIGHT + 20;

	
	int margin_top = FR_MARGIN_TOP + (TEXTBOX_HEIGHT + 20);

	int incr_y = 0;


	for (int i = 0; i < MAX_ACTIVE_USERS; i++) {
		players->base[i].rect = (SDL_FRect) {
			.h = TEXTBOX_HEIGHT,
			.w = TEXTBOX_WIDTH,
			.x = FR_MARGIN_LEFT + 10,
			.y = margin_top + incr_y,
		};
		incr_y += (TEXTBOX_HEIGHT + 20);
	}


}

static void render(App* app) {
	SDL_Renderer* renderer = app->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	text_render(app, &(app->frames.select_opponent.title_component));
	render_players(app);
}

static void handle_up(App* app) {
	player_component_list* players = &(app->frames.select_opponent.player_components);
	int selected = players->selected_index;

	if (selected > 0) {
		players->selected_index--;
	}
}


static void handle_down(App* app) {
	player_component_list* players = &(app->frames.select_opponent.player_components);

	int selected = players->selected_index;
	int len = players->len;

	if (selected < len - 1) {
		players->selected_index++;
	}
}

static void handle_return(App* app) {
	player_component_list* players = &(app->frames.select_opponent.player_components);

	// return early if no players in list
	if (players->len == 0) return;
	
	char* opp_name = players->base[players->selected_index].text;
		
	strncpy((char*)&(app->opponent_username), opp_name, MAX_USERNAME_CHARS);

	tcp_write_msg_1("selected-opponent", opp_name);

	app->current_frame = gameplay;

}


static void input(App* app, SDL_Event* event) {
	if (event->type == SDL_EVENT_KEY_DOWN) {
		printf("keydown event\n");
		switch (event->key.key) {
			case SDLK_UP:
				handle_up(app);
				break;
			case SDLK_DOWN:
				handle_down(app);
				break;
			case SDLK_RETURN:
				handle_return(app);
				break;
		}
	} 
	return;
}

extern fr_handler h_select_opponent = (fr_handler){
	.init = (init_handler*)init,
	.input = (input_handler*)input,
	.render = (render_handler*)render,
};
