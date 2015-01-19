#include "ratium.h"

void move_player(char c, entity_t entity) {

	switch (c) {
		case 'h': --entity.x; break;
		case 'j': ++entity.y; break;
		case 'k': --entity.y; break;
		case 'l': ++entity.x; break;
	}

	/* if (c == 'h')        { entity.x--; */
	/* } else if (c == 'j') { entity.y++; */
	/* } else if (c == 'k') { entity.y--; */
	/* } else if (c == 'l') { entity.x++; */
	/* } */

	mvaddch(entity.y, entity.x, entity.face);

}
