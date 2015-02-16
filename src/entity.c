#include "ratium.h"

bool can_step(entity_t *e, int x, int y) {
	for (int i = 0; i <= entqty; i++)
		if (entity[i].hp > 0)
			/* TODO; Check if holding item */
			if (entity[i].x == x && entity[i].y == y) {
				/* TODO; Improve checking if entity should attack or not */
				if (e->face == '@')
					attack(e, &entity[i]);
				return false;
			}
	for (int i = 0; i <= playerqty; i++)
		if (entity[i].hp > 0)
			if (player[i].x == x && player[i].y == y) {
				attack(e, &player[i]);
				return false;
			}
	switch (get_map(x, y)) {
		case '#': return false;
		case 'w': return false;
		case '+': return false;
		default: return true;
	}
}

void move_entity(entity_t *e, int dx, int dy) {
	if (can_step(e, e->x + dx, e->y + dy)) {
		e->x += dx;
		e->y += dy;
	}
}

void attack(entity_t *e, entity_t *foe) {
	if (e->passive != 0)
		foe->hp -= (e->holding.type == ITEM_SWORD) ?
			    e->damage + e->holding.stat : e->damage;
}

void draw_ent(entity_t e, entity_t oe, int r) {
	if (e.hp > 0)
		if (oe.x-r < e.x && oe.x+r > e.x)
			if (oe.y-r < e.y && oe.y+r > e.y) {
				mvprintw(e.bary, 0, "HP: %d", e.hp);
				mvaddch(e.y, e.x, e.face + e.color);
				if (e.holding.face != ' ')
					switch (e.direc) {
						case LEFT: mvaddch(e.y, e.x-1, e.holding.face + e.holding.color); break;
						case DOWN: mvaddch(e.y+1, e.x, e.holding.face + e.holding.color); break;
						case UP: mvaddch(e.y-1, e.x, e.holding.face + e.holding.color); break;
						case RIGHT: mvaddch(e.y, e.x+1, e.holding.face + e.holding.color); break;
					}
			}
}

/* TODO: Fix Speed */
void rand_ai(entity_t *e, int speed) {
	if (e->hp > 0) {

		int direc = rand() % speed;

		switch (direc) {
			case 0: move_entity(e, -1,  0); break;
			case 1: move_entity(e,  0,  1); break;
			case 2: move_entity(e,  0, -1); break;
			case 3: move_entity(e,  1,  0); break;
		}

	} else if (!e->isdead) {
		e->isdead = true;
		add_item(&item[query_item(e->drop)], e->x, e->y);
	}
}

void dumb_ai(entity_t *e, int xNew, int yNew, int speed) {
	if (e->hp > 0) {

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

	} else if (!e->isdead) {
		e->isdead = true;
		add_item(&item[query_item(e->drop)], e->x, e->y);
	}
}

