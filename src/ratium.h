#ifndef RATIUM_H
#define RATIUM_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RED COLOR_PAIR(1)
#define GREEN COLOR_PAIR(2)
#define BLUE COLOR_PAIR(3)
#define YELLOW COLOR_PAIR(4)
#define BROWN COLOR_PAIR(5)
#define GREY COLOR_PAIR(6)
#define DARK_YELLOW COLOR_PAIR(8)
#define WATER COLOR_PAIR(9)
#define GRASS COLOR_PAIR(10)

#define MAX_ENTITIES 256
#define MAX_ITEMS 256

#define gold item[0]
#define door item[1]

typedef struct {
	char map[24][81];
	char face;
	int color;
} item_t;

typedef struct {
	char face;
	int color;
	int x, y;
	int oldx, oldy;

	int maxhp, hp;
	int damage;
	int gold;
	char holding;
} entity_t;

/* map.c */
char get_map(int x, int y);
void draw_map();

/* item.c */
void alloc_item(item_t *item);
void toggle_door(int x, int y);
void clear_item(item_t *item, int x, int y);
void draw_item(item_t item);

/* entity.c */
void init_entity(int from, int to);
bool can_step(int x, int y);
void move_entity(entity_t *e, int x_0, int y_0);
void attack(entity_t *e, entity_t *foe);
void rand_ai(entity_t *e, int speed);
void dumb_ai(entity_t *e, int xNew, int yNew, int speed);

/* player.c */
void get_item(entity_t *e);
void player_run(char c, entity_t *e);

item_t item[MAX_ITEMS];
entity_t entity[MAX_ENTITIES];

#endif
