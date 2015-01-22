#include "ratium.h"

void get_item(entity_t *e) {
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
		case 'g': get_item(e); break;
		case 'o': toggle_door(e->x, e->y); break;
	}

	mvprintw(0, 0, "Gold:%2d", e->gold);
	mvaddch(e->y, e->x, e->face + e->color);

}
