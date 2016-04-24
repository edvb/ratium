/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"

static struct _Keys player_keys[MAX_PLAYERS] = {
{ SDL_SCANCODE_H, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_I },
{ SDL_SCANCODE_A, SDL_SCANCODE_X, SDL_SCANCODE_W, SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_T, SDL_SCANCODE_V },
};

struct Ent_t {
	char *name, *drop, *msg; /* TODO: Make msg (& drop?) an array */
	char face;
	int color;
	int hp;
	int damage;
	int sight;
	float speed;
	ENT_TYPE type;
	ENT_AI ai;
	int rarity;
};

static struct Ent_t player_t[MAX_PLAYERS] = {
{ "player1", NULL, NULL, '@', 3, 10, 1, 16, .5, TYPE_ALL, AI_PLAYER, 0 },
{ "player2", NULL, NULL, '@', 1, 10, 1, 16, .5, TYPE_ALL, AI_PLAYER, 0 },
};

static struct Ent_t ent_t[MAX_ENTITIES] = {
{ "rat", "rat meat", NULL,
  'r', 5, 2,  1, 4, 6, TYPE_CAVE,  AI_HOSTILE, 5 },
{ "super rat", "rat meat", NULL,
  'R', 5, 4,  2, 8, 5,  TYPE_CAVE, AI_HOSTILE, 3 },
{ "gnu", "gnu meat", NULL,
  'G', 6, 6,  1, 3, 8,  TYPE_GRASS, AI_PEACEFUL, 2 },
{ "cow", "beef", NULL,
  'c', 5, 2,  0, 3, 6,  TYPE_GRASS, AI_PEACEFUL, 5 },
{ "King Arthur", "gold", "I am King Arthur",
  '@', 4, 10, 0, 16, 12, TYPE_ALL, AI_PEACEFUL, 1 },
{ "knight", "sword", "Who goes there?",
  '@', 6, 10, 0, 16, 12, TYPE_ALL, AI_PEACEFUL, 2 },
{ "peasant", NULL, "Oh. How'd you do",
  '@', 5, 10, 0, 16, 12, TYPE_ALL, AI_PEACEFUL, 2 },
{ "peasant", NULL, "lovely filth down here",
  '@', 5, 10, 0, 16, 12, TYPE_ALL, AI_PEACEFUL, 2 },
};
int entqty_t = 8;

struct {
	char *name;
	char face;
	int stat;
} block_t[MAX_ITEMS] = {
{ "rock",        '#', 0 },
{ "stone",       '.', 0 },
{ "grass",       'g', 0 },
{ "plank",       'X', 0 },
{ "water",       'w', 0 },
{ "door_closed", '+', 0 },
{ "door_open",   '-', 0 },
};
int blockqty_t = 7;

struct {
	char *name;
	ITEM_TYPE type;
	int stat;
	int rarity;
} item_t[MAX_ITEMS] = {
{ "gold",     0, 0,  5 },
{ "spam",     1, 2,  4 },
{ "haggis",   1, 10, 1 },
{ "sword",    2, 2,  3 },
{ "shield",   3, 4,  2 },
{ "bow",      4, 0,  3 },
{ "arrow",    5, 5,  4 },
{ "rat meat", 1, -1, 0 },
{ "gnu meat", 1, 4,  0 },
{ "beef",     1, 22, 0 },
};
int itemqty_t = 10;

/* TODO: Improve and implement in other functions */
/* TODO: Add min and max parameters */
/* calc_rarity: change rarity to make it random */
static void
calc_rarity(int *rarity) {
	if (*rarity != 0)
		switch (rand() % 2) {
		case 0: *rarity += rand() % 3;
		case 1: *rarity -= rand() % 3;
		}
}

/* gen_ent: change x and y values to a valid place for entity to be generated
 *          based on type */
static void
gen_ent(float *x, float *y, ENT_TYPE type) {
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

bool
init_block(void) {
	blockqty = 0;

	for (int num = 0; num < blockqty_t; num++) {
		block[blockqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(block[blockqty].name, block_t[blockqty].name);
		block[blockqty].face = block_t[blockqty].face;
		block[blockqty].stat = block_t[blockqty].stat;

		char imgpath[64] = {0};
		sprintf(imgpath, "data/blocks/%s.png", block[blockqty].name);
		block[blockqty].img = load_img(imgpath);
		block[blockqty].src = (SDL_Rect) { 0, 0, U, U };

		blockqty++;
	}

	blockqty++;

	return true;
}

/* init_item: copies values from item_t[] to item[] */
bool init_item(void) {
	itemqty = 0;

	for (int num = 0; num < itemqty_t; num++) {
		item[itemqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(item[itemqty].name, item_t[itemqty].name);
		item[itemqty].type = item_t[itemqty].type;
		item[itemqty].stat = item_t[itemqty].stat;

		char imgpath[64] = {0};
		sprintf(imgpath, "data/items/%s.png", item[itemqty].name);
		item[itemqty].img = load_img(imgpath);
		item[itemqty].src = (SDL_Rect) { 0, 0, U, U };

		for (int i = 0; i < MAX_X; i++)
			for (int j = 0; j < MAX_Y; j++)
				item[itemqty].map[j][i] = 0;

		calc_rarity(&item_t[itemqty].rarity);
		for (int x, y, i = 0; i < item_t[itemqty].rarity; i++) {
			do {
				x = rand() % MAX_X;
				y = rand() % MAX_Y;
			} while (get_map(x, y) != '.');
			item[itemqty].map[y][x]++;
		}

		itemqty++;
	}

	itemqty++;

	return true;
}

bool /* copies values from ent_t[] to entity[] */
init_entity(void) {
	entqty = 0;
	for (int i = 0; i < entqty_t; i++) {
		calc_rarity(&ent_t[i].rarity);
		for (int num = 0; num < ent_t[i].rarity; num++, entqty++) {
			entity[entqty].name = malloc(MAX_NAME * sizeof(char));
			strcpy(entity[entqty].name, ent_t[i].name);
			entity[entqty].type = ent_t[i].type;
			entity[entqty].ai = ent_t[i].ai;
			entity[entqty].maxhp = ent_t[i].hp;
			entity[entqty].hp = ent_t[i].hp;
			entity[entqty].isdead = false;
			entity[entqty].damage = ent_t[i].damage;
			entity[entqty].sight = ent_t[i].sight;

			char imgpath[64] = {0};
			sprintf(imgpath, "data/ents/%s.png", entity[entqty].name);
			entity[entqty].img = load_img(imgpath);
			entity[entqty].src = (SDL_Rect) { 0, 0, U, U };
			entity[entqty].flip = SDL_FLIP_NONE;

			entity[entqty].speed = ent_t[i].speed;

			entity[entqty].hand = -1;

			entity[entqty].msg = malloc(MAX_NAME * sizeof(char));
			if (ent_t[i].msg != NULL) {
				strcpy(entity[entqty].msg, ent_t[i].msg);
			} else
				entity[entqty].msg = NULL;

			for (int j = 0; j < MAX_INV; j++) {
				entity[entqty].inv[j].name = malloc(MAX_NAME * sizeof(char));
				entity[entqty].inv[j].map[0][0] = 0;
			}

			if (ent_t[i].drop != NULL) {
				strcpy(entity[entqty].inv[0].name, ent_t[i].drop);
				entity[entqty].inv[0].map[0][0] = rand() % 3;
			}

			gen_ent(&entity[entqty].pos.x, &entity[entqty].pos.y, ent_t[i].type);
			entity[entqty].pos.w = 1.0;
			entity[entqty].pos.h = 1.0;

			switch(ent_t[i].ai) {
			case AI_PLAYER: break;
			case AI_HOSTILE:
				entity[entqty].run = dumb_ai;
				break;
			case AI_PEACEFUL:
				entity[entqty].run = rand_ai;
				break;
			}

		}
	}

	return true;
}

/* init_player: copies values from player_t[] to player[] */
bool init_player(int count) {
	int x_0, y_0;

	for (int num = 0; num < count; num++) {
		player[num].name = malloc(MAX_NAME * sizeof(char));
		strcpy(player[num].name, player_t[num].name);
		player[num].type = TYPE_ALL;
		player[num].ai = AI_PLAYER;

		player[num].img = load_img("data/ents/player.png");
		player[num].src = (SDL_Rect) { 0, 0, U, U };
		player[num].flip = SDL_FLIP_NONE;

		player[num].maxhp = player_t[num].hp;
		player[num].hp = player[num].maxhp;
		player[num].isdead = false;
		player[num].damage = player_t[num].damage;
		player[num].sight = player_t[num].sight;
		player[num].speed = player_t[num].speed;

		player[num].direc = RIGHT;
		do {
			x_0 = rand() % MAX_X;
			y_0 = rand() % MAX_Y;
		} while (!is_floor(x_0, y_0));
		player[num].pos.x = x_0;
		player[num].pos.y = y_0;
		player[num].pos.w = 1.0;
		player[num].pos.h = 1.0;
		player[num].bary = num;

		player[num].keys = player_keys[num];

		player[num].msg = (char *)malloc(MAX_NAME * sizeof(char));
		player[num].msg = NULL;

		for (int i = 0; i < MAX_INV; i++) {
			player[num].inv[i].name = malloc(MAX_NAME * sizeof(char));
			player[num].inv[i].face = ' ';
			player[num].inv[i].map[0][0] = 0;
		}

		player[num].hand = -1;

		player[num].run = NULL;
	}

	playerqty = count-1;

	return true;
}

