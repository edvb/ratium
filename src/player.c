#include "ratium.h"

bool can_step(int x, int y) {
	if (mvinch(y, x) == '.') {
		return true;
	} else if (mvinch(y, x) == '.' + COLOR_PAIR(10)) {
		return true;
	} else if (mvinch(y, x) == '$' + COLOR_PAIR(4)) {
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

void player_run(char c, entity_t *e) {

	switch (c) {
		case 'h': move_entity(e, -1,  0); break;
		case 'j': move_entity(e,  0,  1); break;
		case 'k': move_entity(e,  0, -1); break;
		case 'l': move_entity(e,  1,  0); break;
	}

	/* switch (c) { */
	/* 	case 'h': e->x--; break; */
	/* 	case 'j': e->y++; break; */
	/* 	case 'k': e->y--; break; */
	/* 	case 'l': e->x++; break; */
	/* } */

	/* mvaddch(e->y, e->x, e->face + e->color); */

}
