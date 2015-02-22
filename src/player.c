#include "ratium.h"

void get_item(entity_t *e) {
	for (int i = 0; i <= itemqty; i++)
		if (item[i].map[e->y][e->x] == item[i].face) {
			inv_add_item(e, &item[i], 1);
			clear_item(&item[i], e->x, e->y);
			return;
		}
	/* TODO: Break into function */
	/* if nothing is under player put what player was holding into inv */
	if (e->holding.face != ' ') {
		e->holding.face = ' ';
		e->holding.color = 0;
		e->holding.type = 0;
		e->holding.stat = 0;
		for (int i = 0; i < MAX_INV_SLOTS; i++)
			if (strcmp(e->inv[i].name, e->holding.name) == 0)
				e->inv[i].qty++;
	}
}

void player_run(int c, entity_t *e) {
	if (isalive(e->hp)) {

		switch (c) {
			case 'h':
			case KEY_LEFT:
				move_entity(e, -1,  0);
				e->direc = LEFT;
				break;
			case 'j':
			case KEY_DOWN:
				move_entity(e,  0,  1);
				e->direc = DOWN;
				break;
			case 'k':
			case KEY_UP:
				move_entity(e,  0, -1);
				e->direc = UP;
				break;
			case 'l':
			case KEY_RIGHT:
				move_entity(e,  1,  0);
				e->direc = RIGHT;
				break;
			case '.': break;
			case 'g': get_item(e); break;
			case 'o': toggle_door(e->x, e->y); break;
			case 'i': inv(e); break;
		}

	}
}

