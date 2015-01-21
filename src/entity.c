#include "ratium.h"

bool can_step(int x, int y) {
	if (mvinch(y, x) == '.') {
		return true;
	} else if (mvinch(y, x) == '.' + COLOR_PAIR(10)) {
		return true;
	} else if (mvinch(y, x) == '$' + COLOR_PAIR(4)) {
		return true;
	} else if (mvinch(y, x) == '-' + COLOR_PAIR(5)) {
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

