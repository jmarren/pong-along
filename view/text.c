


#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "../app.h"

// static SDL_Texture *texture = NULL;
// static TTF_Font *font = NULL;

// extern unsigned char tiny_ttf[];
// extern unsigned int tiny_ttf_len;
static SDL_Texture *texture = NULL;

void text_render(App* app, char* message, SDL_FRect* rect) {
	SDL_Color color = { 244,  200, 255, SDL_ALPHA_OPAQUE };
	SDL_Surface *text;

	 /* Create the text */
	text = TTF_RenderText_Blended(app->font, message, 0, color);
	if (text) {
	    texture = SDL_CreateTextureFromSurface(app->renderer, text);
	    SDL_DestroySurface(text);
	}
	if (!texture) {
	    SDL_Log("Couldn't create text: %s\n", SDL_GetError());
	}

	
    	SDL_GetTextureSize(texture, &(rect->w), &(rect->h));
	SDL_RenderTexture(app->renderer, texture, NULL, rect);

}
