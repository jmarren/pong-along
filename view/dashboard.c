#include "../app.h"
#include "view.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include "text.h"

#include "../vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"


// #include "sdl_ttf.h"
// #include "../vendored/SDL_ttf/include/"

#define TEXTBOX_W 100
#define TEXTBOX_H 10


void init_textbox(App* app) {
	SDL_FRect container;
	container.x = (WINDOW_W / 2.0) - (TEXTBOX_W / 2.0);
	container.y = 100;
	container.w = TEXTBOX_W;
	container.h = TEXTBOX_H;

	app->dashboard_textbox_container = container;

	SDL_Rect textbox;

	textbox.x = (WINDOW_W / 2.0) - (TEXTBOX_W / 2.0);
	textbox.y = 100;
	textbox.w = TEXTBOX_W;
	textbox.h = TEXTBOX_H;

	app->dashboard_textbox = &textbox;

}


void dashboard_init(App* app) {
	init_textbox(app);
}


void render_textbox(App* app) {
	SDL_RenderRect(app->renderer, &(app->dashboard_textbox_container));
	SDL_SetTextInputArea(app->window, app->dashboard_textbox, 4);
	// printf("app->text_input = %s\n", app->text_input);
	text_render(app, app->text_input);
}

void dashboard_render(App* app) {
	render_textbox(app);
}




