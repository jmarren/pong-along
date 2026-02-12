
#include "circle.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#include "physics.h"
#include "view.h"

Circle circle;


int status;
int d;

int offsetx;
int offsety;

Circle* circle_init(void) {
	circle.radius = 50;
	circle.obj.pos.x = 78;
	circle.obj.pos.y = 100;
	circle.obj.direction = 0.21 * M_PI;
	circle.obj.speed = 0;
	return &circle;
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


void circle_set_y(int y) {
	circle.obj.pos.y = y;
}


void circle_bounce_wall_left_right(void) {
	circle.obj.direction = M_PI - circle.obj.direction;
}

void circle_bounce_wall_top_bottom(void) {
	circle.obj.direction = 2 * M_PI - circle.obj.direction;
}


void circle_move(SDL_Renderer * renderer)  {
	if (circle.obj.pos.x + circle.radius  >= WINDOW_W) {
		circle.obj.pos.x = WINDOW_W - circle.radius;
		circle_bounce_wall_left_right();

	} else if (circle.obj.pos.x  - circle.radius <= 0) {
		circle.obj.pos.x = circle.radius;
		circle_bounce_wall_left_right();
	} else if (circle.obj.pos.y + circle.radius >= WINDOW_H) { 
		circle.obj.pos.y = WINDOW_H - circle.radius;
		circle_bounce_wall_top_bottom();

	} else if (circle.obj.pos.y - circle.radius <= 0) {
		circle.obj.pos.y = circle.radius;
		circle_bounce_wall_top_bottom();
	}

	physics_move_obj((Object *)&circle.obj);
}


