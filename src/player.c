#include "ratium.h"

void player_run(char c, entity_t *e) {

	switch (c) {
		case 'h': e->x--; break;
		case 'j': e->y++; break;
		case 'k': e->y--; break;
		case 'l': e->x++; break;
	}

	/* mvaddch(e->y, e->x, e->face + e->color); */

}
