


#include <SDL3/SDL_render.h>
#include "../client.h"

void render_line(SDL_Renderer* renderer, line* line) {
	SDL_RenderLine(renderer, line->originx, line->originy, line->tailx, line->taily);
}
