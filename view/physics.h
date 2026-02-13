#ifndef PHYSICS_H
#define PHYSICS_H


typedef struct {
	float x;
	float y;
} Position;

typedef struct {
	float direction; // in radians
	Position pos;
	int speed;
} Object;

void physics_move_obj(Object* obj);

#endif
