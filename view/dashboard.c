#include "../app.h"
#include "view.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include "text.h"



#define TEXTBOX_W 1000
#define TEXTBOX_H 10


void init_textbox(App* app) {
	SDL_FRect* container = &(app->dashboard_textbox);
	container->x = (WINDOW_W / 2.0) - (TEXTBOX_W / 2.0);
	container->y = 100;
	container->w = TEXTBOX_W;
	container->h = TEXTBOX_H;
}

void init_title(App* app) {
	SDL_FRect* title = &(app->dashboard_title);
	title->x = app->dashboard_textbox.x;
	title->y = app->dashboard_textbox.y - 20;
	title->w = TEXTBOX_W;
	title->h = TEXTBOX_H;
}



void dashboard_init(App* app) {
	init_textbox(app);
	init_title(app);
}


void render_textbox(App* app) {

	text_render(app, "enter your username", &(app->dashboard_title));
	
	// SDL_RenderRect(app->renderer, app->dashboard_textbox);
	if (strlen(app->text_input) > 0) {
		// SDL_FRect 
		text_render(app, app->text_input, &(app->dashboard_textbox));
	}
}

void dashboard_render(App* app) {
	render_textbox(app);
}




