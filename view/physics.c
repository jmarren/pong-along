#include <math.h>
#include "physics.h"

// typedef struct {
// 	int x;
// 	int y;
// } Position;
//
// typedef struct {
// 	int direction; // in radians
// 	Position pos;
// } Object;

void physics_move_obj(Object* obj) {
	obj->pos.x += cos(obj->direction) * obj->speed;
	obj->pos.y += sin(obj->direction) * obj->speed;
}



// void physics_collide(Object* a, Object* b) {
//
// }



