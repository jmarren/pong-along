#include <math.h>
#include <stdio.h>
#include "physics.h"

void physics_move_obj(Object* obj) {
	obj->pos.x += cos(obj->direction) * obj->speed;
	obj->pos.y += sin(obj->direction) * obj->speed;
}

// void reverse_




