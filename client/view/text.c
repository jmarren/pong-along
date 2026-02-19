


#include "text.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../../vendored/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "../client.h"

// static SDL_Texture *texture = NULL;
// static TTF_Font *font = NULL;

// extern unsigned char tiny_ttf[];
// extern unsigned int tiny_ttf_len;

void text_render(App* app, text_component* component) {
	if (strlen(component->text) <= 0) return;

	SDL_Color color = { 244,  200, 255, SDL_ALPHA_OPAQUE };
	SDL_Surface *text;
	SDL_Texture *texture = NULL;


	 /* Create the text */
	text = TTF_RenderText_Blended(app->font, component->text, 0, color);
	if (text) {
	    texture = SDL_CreateTextureFromSurface(app->renderer, text);
	    SDL_DestroySurface(text);
	}
	if (!texture) {
	    SDL_Log("Couldn't create text: %s\n", SDL_GetError());
	}

	
    	SDL_GetTextureSize(texture, &(component->rect.w), &(component->rect.h));
	SDL_RenderTexture(app->renderer, texture, NULL, &(component->rect));

}
