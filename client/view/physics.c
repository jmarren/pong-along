#include <math.h>
#include <stdio.h>
#include "physics.h"

void physics_move_obj(Object* obj) {
	printf("moving object\n");
	printf("obj->pos.x = %f\n", obj->pos.x);
	printf("obj->pos.y = %f\n", obj->pos.y);
	obj->pos.x += cos(obj->direction) * obj->speed;
	obj->pos.y += sin(obj->direction) * obj->speed;
	printf("obj->pos.x = %f\n", obj->pos.x);
	printf("obj->pos.y = %f\n", obj->pos.y);
	

}




