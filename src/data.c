#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"

static struct _Keys player_keys[MAX_PLAYERS] = {
{ 'h', 'j', 'k', 'l', 'b', 'y', 'n', 'u', '.', 'g', 'o', 'p', 'i' },
{ 'a', 'x', 'w', 'd', 'z', 'q', 'c', 'e', 's', 'r', 'f', 't', 'v' },
};

struct Ent_t {
	char *name;
	int color;
	int hp;
	int damage;
	int sight;
};

static struct Ent_t player_t[MAX_PLAYERS] = {
{ "player1", 3, 10, 1, 16 },
{ "player2", 4, 10, 1, 16 },
};

/* TODO: Improve and implement in other functions */
/* TODO: Add min and max parameters */
/* calc_rarity: change rarity to make it random */
static void
calc_rarity(int *rarity) {
	if (*rarity != 0) {
		*rarity += rand() % 3;
		if (*rarity + entqty > MAX_ENTITIES)
			*rarity = 0;
	}
}

/* us_to_space: convert underscores in string name to spaces */
static void
us_to_space(char *data) {
	for(int i = 0, l = strlen(data); i < l; i++)
		if(data[i] == '_') {
			data[i] = ' ';
			continue;
		}
}

/* gen_ent: change x and y values to a valid place for entity to be generated
 *          based on type */
static void
gen_ent(int *x, int *y, ENT_TYPE type) {
	int spawntile;

	switch(type) {
	case TYPE_ALL:                    break;
	case TYPE_CAVE:  spawntile = '.'; break;
	case TYPE_GRASS: spawntile = 'g'; break;
	case TYPE_WATER: spawntile = 'w'; break;
	}

	do {
		*x = rand() % MAX_X;
		*y = rand() % MAX_Y;
	} while ((type == TYPE_ALL) ?
		  !is_floor(*x, *y) :
		  get_map(*x, *y) != spawntile);
}

/* init_item: read from data/items.txt file and store in item array */
bool init_item(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char face;
	int color;
	int type;
	int stat;
	int rarity;
	itemqty = 0;

	FILE *f = fopen("data/items.txt", "r");
	if (f == NULL) return false;

	do {
		fscanf(f, "%s %c(%i): type=%i stat=%i rarity=%i\n",
			   name, &face, &color, &type, &stat, &rarity);

		us_to_space(name);

		item[itemqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(item[itemqty].name, name);
		item[itemqty].face = face;
		item[itemqty].color = color;
		item[itemqty].type = type;
		item[itemqty].stat = stat;

		for (int i = 0; i < MAX_X; i++)
			for (int j = 0; j < MAX_Y; j++)
				item[itemqty].map[j][i] = 0;

		calc_rarity(&rarity);
		for (int x, y, i = 0; i < rarity; i++) {
			do {
				x = rand() % MAX_X;
				y = rand() % MAX_Y;
			} while (get_map(x, y) != '.');
			item[itemqty].map[y][x]++;
		}

		itemqty++;

	} while (!feof(f));

	fclose(f);

	free(name);

	itemqty++;

	return true;
}

/* init_entity: read from data/entities.txt file and store in entity array */
bool init_entity(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char *drop = malloc(MAX_NAME * sizeof(char));
	int type;
	int ai;
	char face;
	int color;
	int maxhp;
	int damage;
	int sight;
	int rarity;

	int x_0, y_0;

	entqty = 0;

	FILE *f = fopen("data/entities.txt", "r");
	if (f == NULL) return false;

	do {
		fscanf(f, "%s %c(%i): hp=%i damge=%i type=%i ai=%i sight=%i rarity=%i %s\n",
			   name, &face, &color, &maxhp, &damage, &type, &ai, &sight, &rarity, drop);

		us_to_space(name);
		us_to_space(drop);

		calc_rarity(&rarity);
		for (int num = 0; num < rarity; num++, entqty++) {
			entity[num].name = malloc(MAX_NAME * sizeof(char));
			strcpy(entity[num].name, name);
			entity[num].type = type;
			entity[num].ai = ai;
			entity[num].face = face;
			entity[num].color = color;
			entity[num].maxhp = maxhp;
			entity[num].hp = maxhp;
			entity[num].isdead = false;
			entity[num].damage = damage;
			entity[num].sight = sight;

			entity[num].speed = 3; /* TODO */

			entity[num].holding.name = malloc(MAX_NAME * sizeof(char));
			entity[num].holding.face = ' ';
			entity[num].holding.color = 0;
			entity[num].holding.type = 0;
			entity[num].holding.stat = 0;

			entity[num].msg.data = malloc(MAX_NAME * sizeof(char));
			entity[num].msg.disp = false;
			if (strcmp(name, "spock") == 0) { /* TODO: Make this not suck */
				strcpy(entity[num].msg.data, "live long and proposer");
				entity[num].msg.disp = true;
			}

			for (int i = 0; i < MAX_INV; i++) {
				entity[num].inv[i].name = malloc(MAX_NAME * sizeof(char));
				entity[num].inv[i].face = ' ';
				entity[num].inv[i].color = 0;
				entity[num].inv[i].map[0][0] = 0;
			}

			if (strcmp(drop, "none") != 0) {
				strcpy(entity[num].inv[0].name, drop);
				entity[num].inv[0].map[0][0] = rand() % 3;
			}

			gen_ent(&x_0, &y_0, type);
			entity[num].x = x_0;
			entity[num].y = y_0;

		}
	} while (!feof(f));

	fclose(f);

	free(name);
	free(drop);

	return true;
}

/* init_player: set up player array with contents of player_t */
bool init_player(int count) {
	int x_0, y_0;

	for (int num = 0; num < count; num++) {
		player[num].name = malloc(MAX_NAME * sizeof(char));
		strcpy(player[num].name, player_t[num].name);
		player[num].type = TYPE_ALL;
		player[num].ai = AI_PLAYER;
		player[num].face = '@';
		player[num].color = player_t[num].color;

		player[num].maxhp = player_t[num].hp;
		player[num].hp = player[num].maxhp;
		player[num].isdead = false;
		player[num].damage = player_t[num].damage;
		player[num].sight = player_t[num].sight;
		player[num].speed = 0;

		player[num].direc = RIGHT;
		do {
			x_0 = rand() % MAX_X;
			y_0 = rand() % MAX_Y;
		} while (!is_floor(x_0, y_0));
		player[num].x = x_0;
		player[num].y = y_0;
		player[num].bary = num;

		player[num].keys = player_keys[num];

		player[num].msg.data = malloc(MAX_NAME * sizeof(char));
		player[num].msg.disp = false;

		for (int i = 0; i < MAX_INV; i++) {
			player[num].inv[i].name = malloc(MAX_NAME * sizeof(char));
			player[num].inv[i].face = ' ';
			player[num].inv[i].color = 0;
			player[num].inv[i].map[0][0] = 0;
		}

		player[num].holding.name = malloc(MAX_NAME * sizeof(char));
		player[num].holding.face = ' ';
		player[num].holding.color = 0;
		player[num].holding.type = 0;
		player[num].holding.stat = 0;

	}

	playerqty = count-1; /* TODO Fix */

	return true;
}

