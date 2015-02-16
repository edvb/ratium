#include "ratium.h"

void get_item(entity_t *e) {
	for (int i = 0; i <= itemqty; i++)
		if (item[i].map[e->y][e->x] == item[i].face) {
			inv_add_item(e, &item[i], 1);
			clear_item(&item[i], e->x, e->y);
			return;
		}
	/* if nothing is under player put what player was holding into inv */
	if (e->holding.face != ' ') {
		e->holding.face = ' ';
		e->holding.color = 0;
		e->holding.type = 0;
		e->holding.stat = 0;
		for (int i = 0; i < MAX_INV_SLOTS; i++)
			if (e->inv[i].name = e->holding.name)
				e->inv[i].qty++;
	}
}

void player_run(char c, entity_t *e) {
	if (e->hp > 0) {

		switch (c) {
			case 'h': move_entity(e, -1,  0); e->direc = LEFT; break;
			case 'j': move_entity(e,  0,  1); e->direc = DOWN; break;
			case 'k': move_entity(e,  0, -1); e->direc = UP; break;
			case 'l': move_entity(e,  1,  0); e->direc = RIGHT; break;
			case 'g': get_item(e); break;
			case 'o': toggle_door(e->x, e->y); break;
			case 'i': inv(e); break;
		}

	}
}

