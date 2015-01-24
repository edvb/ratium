#include "ratium.h"

void init_entity(int from, int to) {

	int x_0, y_0;
	int bary = 25;
	char face;
	int color;
	int maxhp;
	int damage;

	FILE *f = fopen("data/entities.txt", "r");

	for (int i = from; i <= to; i++, bary++) {
		fscanf(f, "%c %i %i %i %i %i\n", &face, &color, &x_0, &y_0, &maxhp, &damage);

		entity[i].face = face;
		entity[i].color = COLOR_PAIR(color);
		entity[i].x = x_0;
		entity[i].y = y_0;
		entity[i].oldx = x_0;
		entity[i].oldy = y_0;
		entity[i].bary = bary;
		entity[i].maxhp = maxhp;
		entity[i].hp = maxhp;
		entity[i].damage = damage;
		entity[i].gold = 0;
		entity[i].holding = ' ';
	}

	fclose(f);

}

bool can_step(int x, int y) {

	switch (get_map(x, y)) {
		case '#': return false;
		case 'w': return false;
	}

	if (door.map[y][x] == '+')
		return false;

}

void move_entity(entity_t *e, int dx, int dy) {
	if (can_step(e->x + dx, e->y + dy)) {
		e->x += dx;
		e->y += dy;
	}
}

void attack(entity_t *e, entity_t *foe) {
		if (foe->hp > 0)
			if (e->x == foe->x)
				if (e->y == foe->y) {
					foe->hp -= e->damage;
					e->x = e->oldx;
					e->y = e->oldy;
				}
}

void rand_ai(entity_t *e, int speed) {

	if (e->hp > 0) {
		int direc = rand() % speed;

		switch (direc) {
			case 0: move_entity(e, -1,  0); break;
			case 1: move_entity(e,  0,  1); break;
			case 2: move_entity(e,  0, -1); break;
			case 3: move_entity(e,  1,  0); break;
		}

		mvaddch(e->y, e->x, e->face + e->color);
	}

}

void dumb_ai(entity_t *e, int xNew, int yNew, int speed) {

	if (e->hp > 0) {

		e->oldx = e->x;
		e->oldy = e->y;

		int shouldMove = rand() % speed;

		if (shouldMove != 0) {
			if (xNew > e->x)
				move_entity(e,  1,  0);
			else if (xNew < e->x)
				move_entity(e, -1,  0);
			if (yNew > e->y)
				move_entity(e,  0,  1);
			else if (yNew < e->y)
				move_entity(e,  0, -1);
		}

		attack(e, &entity[0]);

		mvprintw(e->bary, 0, "HP: %d", e->hp);
		mvaddch(e->y, e->x, e->face + e->color);
	}

}
