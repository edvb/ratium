/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>

#include "ratium.h"
#include "ent.h"
#include "util.h"

static struct _Keys player_keys[MAX_PLAYERS] = {
{ SDL_SCANCODE_H, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_R, SDL_SCANCODE_T, SDL_SCANCODE_E },
{ SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_W, SDL_SCANCODE_D, SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_C },
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

#define PEASANT_MSG "Oh.;How'd you do./Lovely filth;down here"
#define KING_MSG "I am Arthur/King of the Britons"
static struct Ent_t ent_t[MAX_ENTITIES] = {
{ "rat",         "meat",  NULL,              2,  1, 4,  50,  SPAWN_CAVE,  AI_HOSTILE,  5 },
{ "super rat",   "meat",  NULL,              4,  2, 8,  50,  SPAWN_CAVE,  AI_HOSTILE,  3 },
{ "gnu",         "meat",  NULL,              6,  1, 3,  300, SPAWN_GRASS, AI_PEACEFUL, 2 },
{ "cow",         "meat",  "moo",             2,  0, 3,  600, SPAWN_GRASS, AI_PEACEFUL, 5 },
{ "King Arthur", "gold",  KING_MSG,          10, 0, 16, 60,  SPAWN_ALL,   AI_PEACEFUL, 1 },
{ "knight",      "sword", "Who goes there?", 10, 0, 16, 0,   SPAWN_ALL,   AI_NONE,     2 },
{ "peasant",     NULL,    PEASANT_MSG,       10, 0, 16, 90,  SPAWN_ALL,   AI_PEACEFUL, 4 },
};
int entqty_t = 8;

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
	int tries = 0;
	do {
		*x = rand() % MAX_X;
		*y = rand() % MAX_Y;
		if (tries > 100000) return;
		tries++;
	} while (is_spawn(*x, *y, type));
}

void /* spawn entity by name */
add_ent_name(char *name, int x_0, int y_0, int qty) {
	for (int i = 0; i < entqty_t; i++)
		if (estrcmp(ent_t[i].name, name) == 0)
			add_ent(i, x_0, y_0, qty);
}

void /* spawn qty entities with id entnum at x and y postion */
add_ent(int entnum, int x_0, int y_0, int qty) {
	for (int i = 0; i < qty; i++, entqty++) {
		entity[entqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(entity[entqty].name, ent_t[entnum].name);
		entity[entqty].type = ent_t[entnum].type;
		entity[entqty].ai = ent_t[entnum].ai;

		entity[entqty].img = load_img(get_data("gfx/ents/%s.png", entity[entqty].name));
		entity[entqty].src = (SDL_Rect) { 0, 0, U, U };
		entity[entqty].rot = 0;
		entity[entqty].flip = SDL_FLIP_NONE;

		entity[entqty].pos = (Pos) { x_0, y_0, 1, 1 };
		entity[entqty].direc = RIGHT;

		entity[entqty].maxhp = ent_t[entnum].hp;
		entity[entqty].hp = ent_t[entnum].hp;
		entity[entqty].isdead = false;
		entity[entqty].damage = ent_t[entnum].damage;
		entity[entqty].sight = ent_t[entnum].sight;
		entity[entqty].speed = ent_t[entnum].speed;

		entity[entqty].keys = (struct _Keys){0};

		entity[entqty].msg = malloc(MAX_MSG * sizeof(char));
		if (ent_t[entnum].msg != NULL) {
			strcpy(entity[entqty].msg, ent_t[entnum].msg);
		} else
			entity[entqty].msg = NULL;

		for (int j = 0; j < MAX_INV; j++) {
			entity[entqty].inv[j].name = malloc(MAX_NAME * sizeof(char));
			entity[entqty].inv[j].map[0][0] = 0;
		}
		entity[entqty].hand = -1;

		if (ent_t[entnum].drop != NULL) {
			strcpy(entity[entqty].inv[0].name, ent_t[entnum].drop);
			entity[entqty].inv[0].map[0][0] = rand() % 3;
		}

		entity[entqty].t.dmg   = 0;
		entity[entqty].t.msg   = 0;
		entity[entqty].t.swing = 0;

		switch(ent_t[entnum].ai) {
		case AI_NONE:     entity[entqty].run = no_ai;   break;
		case AI_HOSTILE:  entity[entqty].run = dumb_ai; break;
		case AI_PEACEFUL: entity[entqty].run = rand_ai; break;
		case AI_SHOT:
		case AI_PLAYER: break;
		}
	}
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

			entity[entqty].img = load_img(get_data("gfx/ents/%s.png", entity[entqty].name));
			entity[entqty].src = (SDL_Rect) { 0, 0, U, U };
			entity[entqty].rot = 0;
			entity[entqty].flip = SDL_FLIP_NONE;

			entity[entqty].maxhp = ent_t[i].hp;
			entity[entqty].hp = ent_t[i].hp;
			entity[entqty].isdead = false;
			entity[entqty].damage = ent_t[i].damage;
			entity[entqty].sight = ent_t[i].sight;
			entity[entqty].speed = ent_t[i].speed;

			gen_ent(&entity[entqty].pos.x, &entity[entqty].pos.y, ent_t[i].type);
			entity[entqty].pos.w = 1.0;
			entity[entqty].pos.h = 1.0;

			entity[entqty].keys = (struct _Keys) {0};

			entity[entqty].msg = malloc(MAX_MSG * sizeof(char));
			if (ent_t[i].msg != NULL) {
				strcpy(entity[entqty].msg, ent_t[i].msg);
			} else
				entity[entqty].msg = NULL;

			for (int j = 0; j < MAX_INV; j++) {
				entity[entqty].inv[j].name = malloc(MAX_NAME * sizeof(char));
				entity[entqty].inv[j].map[0][0] = 0;
			}
			entity[entqty].hand = -1;

			if (ent_t[i].drop != NULL) {
				strcpy(entity[entqty].inv[0].name, ent_t[i].drop);
				entity[entqty].inv[0].map[0][0] = rand() % 3;
			}

			entity[entqty].t.dmg   = 0;
			entity[entqty].t.msg   = 0;
			entity[entqty].t.swing = 0;

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
		strcpy(player[num].name, "player");
		player[num].type = SPAWN_ALL;
		player[num].ai = AI_PLAYER;

		player[num].img = load_img(get_data("gfx/ents/player.png"));
		player[num].src = (SDL_Rect) { 0, 0, U, U };
		player[num].rot = 0;
		player[num].flip = SDL_FLIP_NONE;

		do {
			x_0 = rand() % MAX_X;
			y_0 = rand() % MAX_Y;
		} while (!is_floor(x_0, y_0));
		player[num].pos.x = x_0;
		player[num].pos.y = y_0;
		player[num].pos.w = 1.0;
		player[num].pos.h = 1.0;
		player[num].direc = RIGHT;

		player[num].maxhp = 10;
		player[num].hp    = player[num].maxhp;
		player[num].isdead = false;
		player[num].damage = 1;
		player[num].sight  = 16;
		player[num].speed  = .25;

		player[num].keys = player_keys[num];

		player[num].msg = (char *)malloc(MAX_NAME * sizeof(char));
		player[num].msg = NULL;

		for (int i = 0; i < MAX_INV; i++) {
			player[num].inv[i].name = malloc(MAX_NAME * sizeof(char));
			player[num].inv[i].face = ' ';
			player[num].inv[i].map[0][0] = 0;
		}
		player[num].hand = -1;

		player[num].t.dmg   = 0;
		player[num].t.msg   = 0;
		player[num].t.swing = 0;

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

	entity[entqty].img = load_img(get_data("gfx/items/%s.png", ammo));
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
	entity[entqty].sight = 0;
	entity[entqty].speed = .5;

	entity[entqty].t.dmg   = 0;
	entity[entqty].t.msg   = 0;
	entity[entqty].t.swing = 0;

	entity[entqty].run = shot_ai;

	entqty++;
}

