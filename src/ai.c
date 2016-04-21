/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"
#include "ent.h"

void
rand_ai(Ent *e) {
	if (isalive(e->hp)) {
		int direc = rand() % e->speed;

		switch (direc) {
		case 0: move_entity(e, -1,  0); break;
		case 1: move_entity(e,  0,  1); break;
		case 2: move_entity(e,  0, -1); break;
		case 3: move_entity(e,  1,  0); break;
		}

	} else if (!e->isdead) { /* TODO: Remove isdead var, use hp var at -1 instead */
		e->isdead = true;
		for (int i = 0; i < MAX_INV; i++)
			for (;e->inv[i].map[0][0] > 0; e->inv[i].map[0][0]--)
				add_item(&item[query_item(e->inv[i].name)], e->x, e->y);
	}
}

void
dumb_ai(Ent *e) {
	int shouldMove;

	if (!isalive(e->hp)) {
		if (!e->isdead) {
			e->isdead = true;
			for (int i = 0; i < MAX_INV; i++)
				for (;e->inv[i].map[0][0] > 0; e->inv[i].map[0][0]--)
					add_item(&item[query_item(e->inv[i].name)], e->x, e->y);
		}
		return;
	}

	if (abs(e->x - player[0].x) <= e->sight && /* TODO: Make dumb AI follow other players */
	    abs(e->y - player[0].y) <= e->sight) {
		if ((shouldMove = rand() % e->speed) != 0)
			return;

		if (player[0].x > e->x)
			move_entity(e, 1, 0);
		else if (player[0].x < e->x)
			move_entity(e, -1, 0);
		if (player[0].y > e->y)
			move_entity(e, 0, 1);
		else if (player[0].y < e->y)
			move_entity(e, 0, -1);
		return;
	} else {
		rand_ai(e);
		return;
	}

}

