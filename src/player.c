#include "ratium.h"

void init_player(int from, int to) {

	int x_0, y_0;
	char face;
	int color;
	int maxhp;
	int damage;
	int passive;

	FILE *f = fopen("data/players.txt", "r");

	for (int i = from; i <= to; i++) {
		fscanf(f, "%c %i %i %i %i %i %i\n",
			   &face, &color, &x_0, &y_0, &maxhp, &damage, &passive);

		player[i].face = face;
		player[i].color = COLOR_PAIR(color);
		player[i].x = x_0;
		player[i].y = y_0;
		player[i].oldx = x_0;
		player[i].oldy = y_0;
		player[i].maxhp = maxhp;
		player[i].hp = maxhp;
		player[i].damage = damage;
		player[i].passive = passive;

		player[i].holding.name = malloc(MAX_NAME * sizeof(char));
		player[i].holding.face = ' ';
		player[i].holding.color = 0;
		player[i].holding.type = 0;
		player[i].holding.stat = 0;

		for (int j = 0; j < 16; j++) {
			player[i].inv[j].name = "";
			player[i].inv[j].face = ' ';
			player[i].inv[j].color = 0;
			player[i].inv[j].qty = 0;
		}
	}

	playerqty = to;

	fclose(f);

}

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

		e->oldx = e->x;
		e->oldy = e->y;

		switch (c) {
			case 'h': move_entity(e, -1,  0); break;
			case 'j': move_entity(e,  0,  1); break;
			case 'k': move_entity(e,  0, -1); break;
			case 'l': move_entity(e,  1,  0); break;
			case 'g': get_item(e); break;
			case 'o': toggle_door(e->x, e->y); break;
			case 'i': inv(e); break;
		}

		for (int i = 0; i < MAX_ENTITIES; i++)
			attack(e, &entity[i]);

	}
}

