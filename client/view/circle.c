
#include "circle.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include "physics.h"
#include "view.h"




void outline_circle(SDL_Renderer* renderer, Circle* circle) {

    int offsetx = 0;
    int offsety = circle->radius;
    int d = circle->radius -1;
    int status = 0;

    while (offsety >= offsetx) {
	
        status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsetx, circle->obj.pos.y + offsety);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsety, circle->obj.pos.y + offsetx);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsetx, circle->obj.pos.y + offsety);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsety, circle->obj.pos.y + offsetx);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsetx, circle->obj.pos.y - offsety);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsety, circle->obj.pos.y - offsetx);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsetx, circle->obj.pos.y - offsety);
        status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsety, circle->obj.pos.y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (circle->radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

}



int fill_circle(SDL_Renderer* renderer, Circle* circle) {

    int offsetx, offsety, d;
    int status;

    int x = circle->obj.pos.x;
    int y = circle->obj.pos.y;


    offsetx = 0;
    offsety = circle->radius;
    d = circle->radius -1;
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
        else if (d < 2 * (circle->radius - offsety)) {
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

void render_circle(SDL_Renderer *renderer, Circle *circle) {
	outline_circle(renderer, circle);
	fill_circle(renderer, circle);
}



// int view_draw_circle(App* app)
// {
//     SDL_Renderer* renderer = app->renderer;
//     Circle* circle = &(app->circle);
//
//     offsetx = 0;
//     offsety = circle->radius;
//     d = circle->radius -1;
//     status = 0;
//
//     while (offsety >= offsetx) {
//
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsetx, circle->obj.pos.y + offsety);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsety, circle->obj.pos.y + offsetx);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsetx, circle->obj.pos.y + offsety);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsety, circle->obj.pos.y + offsetx);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsetx, circle->obj.pos.y - offsety);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x + offsety, circle->obj.pos.y - offsetx);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsetx, circle->obj.pos.y - offsety);
//         status += SDL_RenderPoint(renderer, circle->obj.pos.x - offsety, circle->obj.pos.y - offsetx);
//
//         if (status < 0) {
//             status = -1;
//             break;
//         }
//
//         if (d >= 2*offsetx) {
//             d -= 2*offsetx + 1;
//             offsetx +=1;
//         }
//         else if (d < 2 * (circle->radius - offsety)) {
//             d += 2 * offsety - 1;
//             offsety -= 1;
//         }
//         else {
//             d += 2 * (offsety - offsetx - 1);
//             offsety -= 1;
//             offsetx += 1;
//         }
//     }
//
//     return status;
// }

//
//
// void circle_render(App* app) {
// 	view_draw_circle(app);
// 	view_fill_circle(app);
// }


// void circle_set_y(int y) {
// 	circle.obj.pos.y = y;
// }


void circle_bounce_wall_left_right(Circle* circle) {
	circle->obj.direction = M_PI - circle->obj.direction;
}

void circle_bounce_wall_top_bottom(Circle* circle) {
	circle->obj.direction = 2 * M_PI - circle->obj.direction;
}
//
//
void move_circle(Circle* circle)  {
	 if (circle->obj.pos.y + circle->radius >= WINDOW_H) { 
		circle->obj.pos.y = WINDOW_H - circle->radius;
		circle_bounce_wall_top_bottom(circle);

	} else if (circle->obj.pos.y - circle->radius <= 0) {
		circle->obj.pos.y = circle->radius;
		circle_bounce_wall_top_bottom(circle);
	}

	physics_move_obj((Object *)&(circle->obj));
}
//
//
	// if (circle.obj.pos.x + circle.radius  >= WINDOW_W) {
	// 	circle.obj.pos.x = WINDOW_W - circle.radius;
	// 	circle_bounce_wall_left_right();
	//
	// } else if (circle.obj.pos.x  - circle.radius <= 0) {
	// 	circle.obj.pos.x = circle.radius;
	// 	circle_bounce_wall_left_right();
