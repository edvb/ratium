#include <stdlib.h>

#include "ratium.h"
#include "ent.h"

void rand_ai(Ent *e, int speed) {
	if (isalive(e->hp)) {

		/* TODO: Improve */
		int direc = rand() % speed;

		switch (direc) {
			case 0: move_entity(e, -1,  0); break;
			case 1: move_entity(e,  0,  1); break;
			case 2: move_entity(e,  0, -1); break;
			case 3: move_entity(e,  1,  0); break;
		}

	} else if (!e->isdead) {
		e->isdead = true;
		for (int i = 0; i < MAX_INV; i++)
			while (e->inv[i].map[0][0] > 0) {
				add_item(&item[query_item(e->inv[i].name)],
					 e->x, e->y);
				e->inv[i].map[0][0]--;
			}
	}
}

void dumb_ai(Ent *e, int xNew, int yNew, int speed) {
	if (isalive(e->hp)) {

		if (abs(e->x - xNew) <= e->sight &&
		    abs(e->y - yNew) <= e->sight) {

			int shouldMove = rand() % speed;

			if (shouldMove != 0) {
				if (xNew > e->x)
					move_entity(e, 1, 0);
				else if (xNew < e->x)
					move_entity(e, -1, 0);
				if (yNew > e->y)
					move_entity(e, 0, 1);
				else if (yNew < e->y)
					move_entity(e, 0, -1);
			}
		} else
			rand_ai(e, speed);

	} else if (!e->isdead) {
		e->isdead = true;
		for (int i = 0; i < MAX_INV; i++)
			while (e->inv[i].map[0][0] > 0) {
				add_item(&item[query_item(e->inv[i].name)],
					 e->x, e->y);
				e->inv[i].map[0][0]--;
			}
	}
}

