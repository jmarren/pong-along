#include "../app.h"
#include "view.h"
#include <SDL3/SDL_render.h>

#include "../vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"


// #include "sdl_ttf.h"
// #include "../vendored/SDL_ttf/include/"

#define TEXTBOX_W 100
#define TEXTBOX_H 10


void init_textbox(App* app) {
	
	// TTF_Font

	// TTF_FontHasGlyph;

	app->dashboard_textbox.x = (WINDOW_W / 2.0) - (TEXTBOX_W / 2.0);
	app->dashboard_textbox.y = 100;
	app->dashboard_textbox.w = TEXTBOX_W;
	app->dashboard_textbox.h = TEXTBOX_H;
}


void dashboard_init(App* app) {
	init_textbox(app);
}


void render_textbox(App* app) {
	SDL_RenderRect(app->renderer, &(app->dashboard_textbox));
}

void dashboard_render(App* app) {
	render_textbox(app);
}




