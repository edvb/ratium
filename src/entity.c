#include "ratium.h"

bool can_step(int x, int y) {
	if (mvinch(y, x) == '.') {
		return true;
	} else if (mvinch(y, x) == '.' + GRASS) {
		return true;
	} else if (mvinch(y, x) == '$' + YELLOW) {
		return true;
	} else if (mvinch(y, x) == '-' + BROWN) {
		return true;
	} else {
		return false;
	}
}

void move_entity(entity_t *e, int dx, int dy) {
	if (can_step(e->x + dx, e->y + dy)) {
		e->x += dx;
		e->y += dy;
	}
}

void rand_ai(entity_t *e, int speed) {

	int direc = rand() % speed;

	switch (direc) {
		case 0: move_entity(e, -1,  0); break;
		case 1: move_entity(e,  0,  1); break;
		case 2: move_entity(e,  0, -1); break;
		case 3: move_entity(e,  1,  0); break;
	}

	mvaddch(e->y, e->x, e->face + e->color);

}

void dumb_ai(entity_t *e, int xNew, int yNew, int speed) {

	int shouldMove = rand() % speed;

	if (shouldMove != 0) {
		if (xNew > e->x)
			move_entity(e,  1,  0);
		else if (xNew < e->x)
			move_entity(e, -1,  0);
		if (yNew > e->y)
			move_entity(e,  0,  1);
		else if (yNew < e->y)
			move_entity(e,  0, -1);
	}

	mvaddch(e->y, e->x, e->face + e->color);

}
