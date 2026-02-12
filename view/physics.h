#ifndef PHYSICS_H
#define PHYSICS_H


typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	int direction; // in radians
	Position pos;
	int speed;
} Object;

void physics_move_obj(Object* obj);

#endif
