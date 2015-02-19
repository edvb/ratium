#ifndef RATIUM_H
#define RATIUM_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ever (;;)

#define RED COLOR_PAIR(1)
#define GREEN COLOR_PAIR(2)
#define BLUE COLOR_PAIR(3)
#define YELLOW COLOR_PAIR(4)
#define BROWN COLOR_PAIR(5)
#define GREY COLOR_PAIR(6)
#define DARK_YELLOW COLOR_PAIR(8)
#define WATER COLOR_PAIR(9)
#define GRASS COLOR_PAIR(10)

#define MAX_X 80
#define MAX_Y 24
#define MAX_NAME 16
#define MAX_PLAYERS 8
#define MAX_ENTITIES 256
#define MAX_ITEMS 256
#define MAX_INV_SLOTS 16

int itemqty;
int playerqty;
int entqty;

typedef enum {
	ITEM_MISC,
	ITEM_FOOD,
	ITEM_SWORD,
	ITEM_SHIELD
} item_type;
/* TODO: Change name */

typedef enum {
	LEFT,
	DOWN,
	UP,
	RIGHT,
} DIREC;

/* TODO: Make inv point to a item */
typedef struct {
	char *name;
	char face;
	int color;
	int qty;

	item_type type;
	int stat;
} inv_t;

typedef struct {
	char *name;
	char map[MAX_Y][MAX_X];
	char face;
	int color;

	item_type type;
	int stat;
} item_t;

typedef struct {
	char *name;
	char *drop;
	char face;
	int color;

	DIREC direc;
	int x, y;
	int bary;

	int maxhp, hp;
	bool isdead;
	item_t holding;
	int damage;
	int passive;

	inv_t inv[MAX_INV_SLOTS];
} entity_t;

/***********************\
* map.c: handle the map *
\***********************/
/* get_map: get character of map at x and y position */
char get_map(int x, int y);
/* set_map: set character of map at x and y position to newch */
void set_map(int x, int y, char newch);
/* draw_map: draw the map foreground (stuff that is on top of entities) */
void draw_map(entity_t e, int r);
/* draw_map: draw the map background (stuff that is below entities) */
void draw_map_floor(entity_t e, int r);

/**********************\
* item.c: handle items *
\**********************/
/* query_item: return element number of array matching name supplied */
int query_item(char *name);
/* toggle_door: open or close door next to entity */
void toggle_door(int x, int y);
/* clear_item: clear item at x and y position */
void clear_item(item_t *item, int x, int y);
/* add_item: set item value at x and y position to item face */
void add_item(item_t *item, int x, int y);
/* add_item_ch: set item value at x and y position to newch variable */
void add_item_ch(item_t *item, int x, int y, char newch);
/* draw_item: draw item if in range of entity e by radius of r */
void draw_item(item_t item, entity_t e, int r);

/*****************************************\
* entity.c: handle anything with entities *
\*****************************************/
/* can_step: determine if entity can move to a new space */
bool can_step(entity_t *e, int x, int y);
/* move_entity: move a entity by x_0 and y_0 */
void move_entity(entity_t *e, int x_0, int y_0);
/* attack: entity e attack entity foe */
void attack(entity_t *e, entity_t *foe);
/* isalive: determine if entity is alive */
bool isalive(int hp);
/* holding: return x position for what entity is holding */
int holding_x(entity_t e, int val);
/* holding: return y position for what entity is holding */
int holding_y(entity_t e, int val);
/* draw_ent: draw entity e if in range of entity oe by r */
void draw_ent(entity_t e, entity_t oe, int r);
void rand_ai(entity_t *e, int speed);
void dumb_ai(entity_t *e, int xNew, int yNew, int speed);

/*****************************\
* player.c: handle the player *
\*****************************/
void get_item(entity_t *e);
void player_run(char c, entity_t *e);

/**************************************\
* inv.c: handle the player's inventory *
\**************************************/
void inv(entity_t *e);
void draw_inv(entity_t *e, int arrow_y);
void inv_add_item(entity_t *e, item_t *item, int qty);
void inv_use_item(entity_t *e, int num);
void inv_drop_item(entity_t *e, int num);

/*********************************************\
* data.c: handle reading from data/ directory *
\*********************************************/
/* init_item: read from data/items.txt file and store in item array */
void init_item(int to, int from);
/* init_entity: read from data/entities.txt file and store in entity array */
void init_entity(int from, int to);
/* init_player: read from data/players.txt file and store in player array */
void init_player(int from, int to);

item_t item[MAX_ITEMS];
entity_t player[MAX_PLAYERS];
entity_t entity[MAX_ENTITIES];

#endif
