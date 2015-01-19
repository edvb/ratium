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
} entity_t;

/* map.c */
void draw_map();

/* item.c */
void init_item(item_t item);
void draw_item(item_t item);

/* player.c */
void move_player(char c, entity_t entity);

item_t gold;
entity_t entity[MAX_ENTITIES];

#endif
