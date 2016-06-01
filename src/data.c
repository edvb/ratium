/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>

#include "ratium.h"
#include "ent.h"
#include "util.h"

static struct _Keys player_keys[MAX_PLAYERS] = {
{ SDL_SCANCODE_H, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_I },
{ SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_W, SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_T, SDL_SCANCODE_V },
};

struct Ent_t {
	char *name, *drop, *msg; /* TODO: Make msg (& drop?) an array */
	int hp;
	int damage;
	int sight;
	float speed;
	SpawnType type;
	EntAI ai;
	int rarity;
};

static struct Ent_t player_t[MAX_PLAYERS] = {
{ "player1", NULL, NULL, 10, 1, 8, .5, SPAWN_ALL, AI_PLAYER, 0 },
{ "player2", NULL, NULL, 10, 1, 8, .5, SPAWN_ALL, AI_PLAYER, 0 },
};

static struct Ent_t ent_t[MAX_ENTITIES] = {
{ "rat",         "rat meat", NULL,                     2,  1, 4,  50,  SPAWN_CAVE,  AI_HOSTILE,  5 },
{ "super rat",   "rat meat", NULL,                     4,  2, 8,  50,  SPAWN_CAVE,  AI_HOSTILE,  3 },
{ "gnu",         "gnu meat", NULL,                     6,  1, 3,  300, SPAWN_GRASS, AI_PEACEFUL, 2 },
{ "cow",         "beef",     NULL,                     2,  0, 3,  600, SPAWN_GRASS, AI_PEACEFUL, 5 },
{ "King Arthur", "gold",     "I am King Arthur",       10, 0, 16, 60,  SPAWN_ALL,   AI_PEACEFUL, 1 },
{ "knight",      "sword",    "Who goes there?",        10, 0, 16, 0,   SPAWN_ALL,   AI_NONE,     2 },
{ "peasant",     NULL,       "Oh. How'd you do",       10, 0, 16, 90,  SPAWN_ALL,   AI_PEACEFUL, 2 },
{ "peasant",     NULL,       "lovely filth down here", 10, 0, 16, 90,  SPAWN_ALL,   AI_PEACEFUL, 2 },
};
int entqty_t = 8;

struct {
	char *name;
	char face;
	BlockType type;
	BlockTexType textype;
	BlockClear texclear;
	SpawnType spawntype;
	bool isfloor;
	int stat;
} block_t[MAX_ITEMS] = {
{ "grass",       'g', BLOCK_NORM,  TEX_RAND, CLEAR_NONE, SPAWN_ALL,   true,  4  },
{ "dirt",        'd', BLOCK_NORM,  TEX_XY,   CLEAR_BG,   SPAWN_ALL,   true,  0  },
{ "rock",        '#', BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   false, 0  },
{ "stone",       '.', BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   true,  0  },
{ "plank",       'X', BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   false, 0  },
{ "board",       'b', BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   true,  0  },
{ "water",       'w', BLOCK_WATER, TEX_SXY,  CLEAR_BG,   SPAWN_ALL,   true,  0  },
{ "door_closed", '+', BLOCK_DOOR,  TEX_NORM, CLEAR_FG,   SPAWN_ALL,   false, 0  },
{ "door_open",   '-', BLOCK_DOOR,  TEX_NORM, CLEAR_FG,   SPAWN_ALL,   true,  1  },
{ "window",      'W', BLOCK_NORM,  TEX_NORM, CLEAR_FG,   SPAWN_ALL,   false, 0  },
{ "chair",       'h', BLOCK_NORM,  TEX_NORM, CLEAR_BG,   SPAWN_ALL,   true,  0  },
{ "table",       'o', BLOCK_NORM,  TEX_X,    CLEAR_BG,   SPAWN_ALL,   false, 0  },
{ "barrel",      '0', BLOCK_NORM,  TEX_NORM, CLEAR_BG,   SPAWN_GRASS, false, 0  },
{ "bush",        's', BLOCK_NORM,  TEX_RAND, CLEAR_BG,   SPAWN_GRASS, false, 16 },
{ "flower",      'f', BLOCK_NORM,  TEX_RAND, CLEAR_BG,   SPAWN_GRASS, true,  4  },
{ "tall_grass",  'G', BLOCK_NORM,  TEX_NORM, CLEAR_BG,   SPAWN_GRASS, true,  0  },
};
int blockqty_t = 16;

struct {
	char *name;
	ItemType type;
	int stat;
	int rarity;
} item_t[MAX_ITEMS] = {
{ "gold",     ITEM_MISC,    0,  5 },
{ "spam",     ITEM_FOOD,    2,  4 },
{ "haggis",   ITEM_FOOD,    10, 1 },
{ "sword",    ITEM_SWORD,   2,  3 },
{ "shield",   ITEM_SHIELD,  4,  2 },
{ "bow",      ITEM_SHOOTER, 20, 3 },
{ "arrow",    ITEM_AMMO,    5,  4 },
{ "rat meat", ITEM_FOOD,    -1, 0 },
{ "gnu meat", ITEM_FOOD,    4,  0 },
{ "beef",     ITEM_FOOD,    1,  0 },
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

static void /* change x and y values to a valid location based on spawn type */
gen_ent(float *x, float *y, SpawnType type) {
	do {
		*x = rand() % MAX_X;
		*y = rand() % MAX_Y;
	} while (is_spawn(*x, *y, type));
}

bool
init_block(void) {
	blockqty = 0;

	for (int num = 0; num < blockqty_t; num++) {
		block[blockqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(block[blockqty].name, block_t[blockqty].name);
		block[blockqty].face = block_t[blockqty].face;
		block[blockqty].type = block_t[blockqty].type;
		block[blockqty].textype = block_t[blockqty].textype;
		block[blockqty].texclear = block_t[blockqty].texclear;
		block[blockqty].spawntype = block_t[blockqty].spawntype;
		block[blockqty].isfloor = block_t[blockqty].isfloor;
		block[blockqty].stat = block_t[blockqty].stat;

		char imgpath[64] = {0};
		sprintf(imgpath, "data/blocks/%s.png", block[blockqty].name);
		block[blockqty].img = load_img(imgpath);
		block[blockqty].src = (SDL_Rect) { 0, 0, U, U };

		blockqty++;
	}

	blockqty--;

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
			} while (get_map(x, y) != '.' && get_map(x, y) != 'b');
			if (item_t[itemqty].type == ITEM_AMMO)
				item[itemqty].map[y][x] += 4;
			else
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
			entity[entqty].rot = 0;
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
			case AI_NONE:     entity[entqty].run = no_ai;   break;
			case AI_HOSTILE:  entity[entqty].run = dumb_ai; break;
			case AI_PEACEFUL: entity[entqty].run = rand_ai; break;
			case AI_SHOT:
			case AI_PLAYER: break;
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
		player[num].type = SPAWN_ALL;
		player[num].ai = AI_PLAYER;

		player[num].img = load_img("data/ents/player.png");
		player[num].src = (SDL_Rect) { 0, 0, U, U };
		player[num].rot = 0;
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

void /* populates an entity as a fired shot */
init_shot(Pos pos, Direc direc, int dmg, char *ammo) {
	if (entqty >= MAX_ENTITIES)
		for (int i = 0; i < entqty; i++)
			if (entity[i].ai == AI_SHOT)
				entqty = i;

	entity[entqty].name  = malloc(MAX_NAME * sizeof(char));
	strcpy(entity[entqty].name, ammo);
	entity[entqty].type  = SPAWN_ALL;
	entity[entqty].ai = AI_SHOT;

	char imgpath[64] = {0};
	sprintf(imgpath, "data/items/%s.png", ammo);
	entity[entqty].img = load_img(imgpath);
	entity[entqty].src = (SDL_Rect) { 0, 0, U, U };
	entity[entqty].flip = SDL_FLIP_NONE;

	switch(direc) {
	case LEFT:      entity[entqty].rot = 270; break;
	case DOWN:      entity[entqty].rot = 180; break;
	case UP:        entity[entqty].rot =   0; break;
	case RIGHT:     entity[entqty].rot =  90; break;
	case LEFTDOWN:  entity[entqty].rot = 135; break;
	case LEFTUP:    entity[entqty].rot =  45; break;
	case RIGHTDOWN: entity[entqty].rot = 225; break;
	case RIGHTUP:   entity[entqty].rot = 315; break;
	}

	entity[entqty].pos = (Pos){
		pos.x+holding_x(direc, 0), pos.y+holding_y(direc, 0),
		pos.w, pos.h
	};
	entity[entqty].direc = direc;

	entity[entqty].maxhp = 1;
	entity[entqty].hp = 1;
	entity[entqty].isdead = false;
	entity[entqty].damage = dmg;
	entity[entqty].speed = .5;

	entity[entqty].run = shot_ai;

	entqty++;
}

