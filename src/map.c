/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"
#include "map.h"
#include "gfx.h"

/* find_bld_loc: determines postion for a building on map which it can fit in */
static bool
find_bld_loc(int *x_0, int *y_0, int len, int height) {
	int tries = 0;         /* add 2 for gap around building */
	while (!is_floor_range(*x_0-2, *y_0-2, len+2, height+2)) {
		*x_0 = rand() % MAX_X;
		*y_0 = rand() % MAX_Y;
		if (tries > 100000) /* if it has tried to many times to find a place to fit, give up */
			return false;
		tries++;
	}
	return true;
}

/* init_building: called by init_map for every building in the array and puts
 * them on the map */
static void
draw_building(Map bld) {
	int x_0, y_0;

	switch (rand() % 2) { /* randomly change amount of buildings */
	case 0: bld.rarity += rand() % 2;
	case 1: bld.rarity -= rand() % 2;
	}

	for (int num = 0; num < bld.rarity; num++) {
		if (!find_bld_loc(&x_0, &y_0, bld.len, bld.height))
			return;
		for (int i = 0; i < bld.len; i++) /* copy building to world */
			for (int j = 0; j < bld.height; j++)
				if (bld.map[j][i] != ' ')
					set_map(i+x_0, j+y_0, bld.map[j][i]);
	}
}

static void
draw_room(char wall, char floor, int doorqty, char door) {
	int x_0, y_0;
	int len = rand() % 7 + 4;
	int height = rand() % 7 + 4;

	if (!find_bld_loc(&x_0, &y_0, len, height))
		return;

	for (int i = 0; i < len; i++)
		for (int j = 0; j < height; j++)
			set_map(i+x_0, j+y_0, wall);
	for (int i = 0; i < len-2; i++)
		for (int j = 0; j < height-2; j++)
			set_map(i+x_0+1, j+y_0+1, floor);
	for (int i = 0; i < doorqty; i++)
		switch (rand()%4) {
		case 0:
			set_map(x_0+rand()%len, y_0, door);
			break;
		case 1:
			set_map(x_0, y_0+rand()%height, door);
			break;
		case 2:
			set_map(x_0+rand()%len, y_0+height-1, door);
			break;
		case 3:
			set_map(x_0+len-1, y_0+rand()%height, door);
			break;
		}
}

/* draw buildings and rooms to map */
void init_map(void) {
	for (int i = 0; i < 10; i++) /* create buildings in the world */
		draw_building(buildings[i]);
	for (int i = 0; i < rand()%6; i++) /* create rooms in the world */
		draw_room('X', '.', rand()%3+1, '+');
}

/* get_map: get character of map at x and y position */
char get_map(int x, int y) {
	return worldMap[y][x];
}

/* set_map: set character of map at x and y position to newch */
void set_map(int x, int y, char newch) {
	worldMap[y][x] = newch;
}

/* is_floor: returns true if tile at x and y is a floor tile */
bool is_floor(int x, int y) {
	switch (get_map(x, y)) {
		case '#': return false;
		case 'X': return false;
		case 'w': return false;
		case '+': return false;
		default: return true;
	}
}

/* is_floor_range: return weather or not the area given is all a floor tile */
bool is_floor_range(int x, int y, int dx, int dy) {
	for (int i = x; i <= dx+x; i++)
		for (int j = y; j <= dy+y; j++)
			if (get_map(i, j) != 'g')
				return false;
	return true;
}

/* floor_count: return how many tiles of charater ch are on map */
int floor_count(char ch) {
	int count;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (ch == '*') {
				if (is_floor(i, j)) count++;
			} else {
				if (get_map(i, j) == ch) count++;
			}
	return count;
}

/* toggle_door: open or close door next to entity */
void toggle_door(int x, int y) {
	if      (get_map(x, y) == '+') set_map(x, y, '-');
	else if (get_map(x, y) == '-') set_map(x, y, '+');
}

/* draw_map: draw the map foreground (stuff that is on top of entities) */
void draw_map(Ent e, int r) {
	for (int i = e.x-r; i < e.x+r && i < MAX_X; i++)
		for (int j = e.y-r; j < e.y+r && j < MAX_Y; j++)
			if (j >= 0)
				switch (get_map(i, j)) {
				case '#': rat_mvaddch(i, j, '#', 12); break;
				case 'X': rat_mvaddch(i, j, 'X', 13); break;
				case '+': rat_mvaddch(i, j, '+', 5);  break;
				}
}

/* draw_map: draw the map background (stuff that is below entities) */
void draw_map_floor(Ent e, int r) {
	for (int i = e.x-r; i < e.x+r && i < MAX_X; i++)
		for (int j = e.y-r; j < e.y+r && j < MAX_Y; j++)
			if (j >= 0)
				switch (get_map(i, j)) {
				case ' ': rat_mvaddch(i, j, get_map(i, j), 0); break;
				case '.': rat_mvaddch(i, j, '.', 11); break;
				case 'g': rat_mvaddch(i, j, '.', 10); break;
				case 'w': rat_mvaddch(i, j, '~', 9); break;
				case '-': rat_mvaddch(i, j, '-', 5); break;
				}
}

