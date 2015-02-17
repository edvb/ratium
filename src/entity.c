#include "ratium.h"

bool can_step(entity_t *e, int x, int y) {
	for (int i = 0; i <= entqty; i++)
		/* TODO: Check if holding item */
		if (isalive(entity[i].hp) &&
		    entity[i].x == x && entity[i].y == y) {
			/* TODO: Improve checking if entity should attack or not */
			if (e->face == '@')
				attack(e, &entity[i]);
			return false;
		}
	for (int i = 0; i <= playerqty; i++)
		if (isalive(entity[i].hp) &&
		    player[i].x == x && player[i].y == y) {
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

bool isalive(int hp) {
	return (hp > 0) ? true : false;
}

int holding_x(entity_t e, int val) {
	switch (e.direc) {
		case LEFT:  return val-1;
		case RIGHT: return val+1;
		default:    return val;
	}
}

int holding_y(entity_t e, int val) {
	switch (e.direc) {
		case DOWN:  return val+1;
		case UP:    return val-1;
		default:    return val;
	}
}

void draw_ent(entity_t e, entity_t oe, int r) {
	if (isalive(e.hp) &&
	    oe.x-r < e.x && oe.x+r > e.x &&
	    oe.y-r < e.y && oe.y+r > e.y) {
		mvprintw(e.bary, 0, "HP: %d", e.hp);
		mvaddch(e.y, e.x, e.face + e.color);
		if (e.holding.face != ' ')
			mvaddch(holding_y(e, e.y), holding_x(e, e.x),
				e.holding.face + e.holding.color);
	}
}

/* TODO: Fix Speed */
void rand_ai(entity_t *e, int speed) {
	if (isalive(e->hp)) {

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
	if (isalive(e->hp)) {

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

