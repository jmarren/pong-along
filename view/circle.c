
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#include "physics.h"



typedef struct {
	Object obj;
	int radius;
} Circle;

Circle circle;


int status;
int d;

int offsetx;
int offsety;

void circle_init(void) {
	// circle.x_velocity = 10;
	// circle.y_velocity = 10;
	circle.radius = 50;
	circle.obj.pos.x = 100;
	circle.obj.pos.y = 100;
	circle.obj.direction = 0.5 * M_PI;
	circle.obj.speed = 20;
};


int view_draw_circle(SDL_Renderer * renderer)
{

    offsetx = 0;
    offsety = circle.radius;
    d = circle.radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderPoint(renderer, circle.obj.pos.x + offsetx, circle.obj.pos.y + offsety);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x + offsety, circle.obj.pos.y + offsetx);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x - offsetx, circle.obj.pos.y + offsety);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x - offsety, circle.obj.pos.y + offsetx);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x + offsetx, circle.obj.pos.y - offsety);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x + offsety, circle.obj.pos.y - offsetx);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x - offsetx, circle.obj.pos.y - offsety);
        status += SDL_RenderPoint(renderer, circle.obj.pos.x - offsety, circle.obj.pos.y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (circle.radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int view_fill_circle(SDL_Renderer * renderer)
{
    int offsetx, offsety, d;
    int status;

    int x = circle.obj.pos.x;
    int y = circle.obj.pos.y;


    offsetx = 0;
    offsety = circle.radius;
    d = circle.radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (circle.radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void circle_render(SDL_Renderer * renderer) {
	view_draw_circle(renderer);
	view_fill_circle(renderer);
}


void circle_bounce(void) {
	circle.obj.direction += M_PI;

	// circle.x_velocity *= -1;
	// circle.y_velocity *= 1;
}


void circle_move(SDL_Renderer * renderer)  {
	physics_move_obj((Object *)&circle.obj);
	if (circle.obj.pos.x > 700) {
		circle_bounce();
	}
}


