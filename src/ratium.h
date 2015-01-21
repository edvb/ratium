#ifndef RATIUM_H
#define RATIUM_H

#include <ncurses.h>
#include <stdio.h>

#define MAX_ENTITIES 256

typedef struct {
	char map[24][81];
	char face;
	int color;
} item_t;

typedef struct {
	int x, y;
	char face;
	int color;

	int gold;
	int hp, hpFull;
	int damage;
	char holding;
} entity_t;

/* map.c */
void draw_map();

/* item.c */
void init_item(item_t item);
void clear_item(item_t *item, int x, int y);
void draw_item(item_t item);

/* player.c */
bool can_step(int x, int y);
void move_entity(entity_t *e, int x_0, int y_0);
void get_item(char c, entity_t *e);
void player_run(char c, entity_t *e);

item_t gold;
entity_t entity[MAX_ENTITIES];

#endif
