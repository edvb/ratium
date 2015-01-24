#include "ratium.h"

void get_item(entity_t *e) {
	switch (mvinch(e->y, e->x)) {
		case '$' + COLOR_PAIR(4): e->gold++; clear_item(&gold, e->x, e->y); break;
	}
}

void player_run(char c, entity_t *e) {
	if (e->hp > 0) {

		e->oldx = e->x;
		e->oldy = e->y;

		switch (c) {
			case 'h': move_entity(e, -1,  0); break;
			case 'j': move_entity(e,  0,  1); break;
			case 'k': move_entity(e,  0, -1); break;
			case 'l': move_entity(e,  1,  0); break;
			case 'g': get_item(e); break;
			case 'o': toggle_door(e->x, e->y); break;
		}

		attack(e, &entity[1]);
		attack(e, &entity[2]);

		mvprintw(0, 0, "Gold: %d", e->gold);
		mvprintw(0, 8, "HP: %d", e->hp);
		mvaddch(e->y, e->x, e->face + e->color);

	}
}
