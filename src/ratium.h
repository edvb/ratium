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

#define gold item[0]
#define door item[1]

int itemqty;
int playerqty;
int entqty;

typedef enum {
	ITEM_MISC,
	ITEM_FOOD,
	ITEM_SWORD
} item_type;

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
	char face;
	int color;

	DIREC direc;
	int x, y;
	int oldx, oldy;
	int bary;

	int maxhp, hp;
	int damage;
	item_t holding;
	int passive;

	inv_t inv[MAX_INV_SLOTS];
} entity_t;

/* map.c */
char get_map(int x, int y);
/* TODO: change name */
void set_map(int x, int y);
void set_map_char(int x, int y, char newch);
void draw_map(entity_t e, int r);
void draw_map_floor(entity_t e, int r);

/* item.c */
void alloc_item(int to, int from);
void toggle_door(int x, int y);
void clear_item(item_t *item, int x, int y);
void add_item(item_t *item, int x, int y);
void add_item_ch(item_t *item, int x, int y, char newch);
void draw_item(item_t item, entity_t e, int r);

/* entity.c */
void init_entity(int from, int to);
bool can_step(int x, int y);
void move_entity(entity_t *e, int x_0, int y_0);
void attack(entity_t *e, entity_t *foe);
void draw_ent(entity_t e, entity_t oe, int r);
void rand_ai(entity_t *e, int speed);
void dumb_ai(entity_t *e, int xNew, int yNew, int speed);

/* player.c */
void init_player(int from, int to);
void get_item(entity_t *e);
void player_run(char c, entity_t *e);

/* inv.c */
void inv(entity_t *e);
void draw_inv(entity_t *e, int arrow_y);
void inv_add_item(entity_t *e, item_t *item, int qty);
void inv_use_item(entity_t *e, int num);
void inv_drop_item(entity_t *e, int num);

item_t item[MAX_ITEMS];
entity_t player[MAX_PLAYERS];
entity_t entity[MAX_ENTITIES];

#endif
