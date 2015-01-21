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

void get_item(char c, entity_t *e) {
	switch (mvinch(e->y, e->x)) {
		case '$' + COLOR_PAIR(4): e->gold++; clear_item(&gold, e->x, e->y); break;
	}
}

void player_run(char c, entity_t *e) {

	switch (c) {
		case 'h': move_entity(e, -1,  0); break;
		case 'j': move_entity(e,  0,  1); break;
		case 'k': move_entity(e,  0, -1); break;
		case 'l': move_entity(e,  1,  0); break;
		case 'g': get_item(c, e); break;
	}

	mvprintw(0, 0, "Gold:%2d", e->gold);
	/* mvaddch(e->y, e->x, e->face + e->color); */

}
