#include <ncurses.h>
#include <stdlib.h>

#include "ratium.h"
#include "maps.h"

static void
init_building(Map building) {
	int x_0, y_0;
	int tries = 0;

	building.rarity += rand() % 2; /* randomly change amount of buildings */
	for (int num = 0; num < building.rarity; num++) {
		do {
			x_0 = rand() % MAX_X;
			y_0 = rand() % MAX_Y;
			tries++;
			if (tries > 100000) /* if it has tried to many times to find a place to fit, give up */
				return;
		} while (!is_floor_range(x_0-2, y_0-2, building.len+2, building.height+2)); /* add 2 for gap around building */
		/* copy building to world */
		for (int i = 0; i < building.len; i++)
			for (int j = 0; j < building.height; j++)
				if (building.map[j][i] != ' ')
					set_map(i+x_0, j+y_0, building.map[j][i]);
	}
}

/* init_map: asign values to maprand to determine if character displayed there
 * should be a different char */
void init_map(void) {
	int num;
	for (int i = 0; i < 10; i++) /* create buildings in world */
		init_building(buildings[i]);
	for (int i = 0; i < MAX_X; i++) /* asign random values to maprand, used for added decoration */
		for (int j = 0; j < MAX_Y; j++) {
			if ((num = rand() % 50) == 0)
				maprand[j][i] = 1;
			else
				maprand[j][i] = 0;
		}
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
			if (get_map(i, j) == ch) /* TODO: Incorporate smart is_floor intergration */
				count++;
	return count;
}

/* TODO: Incorporate entity direction */
/* toggle_door: open or close door next to entity */
void toggle_door(int x, int y) {
	if      (get_map(x-1, y) == '+') set_map(x-1, y, '-');
	else if (get_map(x-1, y) == '-') set_map(x-1, y, '+');
	if      (get_map(x+1, y) == '+') set_map(x+1, y, '-');
	else if (get_map(x+1, y) == '-') set_map(x+1, y, '+');
	if      (get_map(x, y-1) == '+') set_map(x, y-1, '-');
	else if (get_map(x, y-1) == '-') set_map(x, y-1, '+');
	if      (get_map(x, y+1) == '+') set_map(x, y+1, '-');
	else if (get_map(x, y+1) == '-') set_map(x, y+1, '+');
}

/* draw_map: draw the map foreground (stuff that is on top of entities) */
void draw_map(Ent e, int r) {
	for (int i = e.x-r; i < e.x+r && i < MAX_X; i++)
		for (int j = e.y-r; j < e.y+r && j < MAX_Y; j++)
			if (j >= 0)
				switch (get_map(i, j)) {
				case '#': mvaddch(j, i, '#' + COLOR_PAIR(12)); break;
				case 'X': mvaddch(j, i, 'X' + COLOR_PAIR(13)); break;
				case '+': mvaddch(j, i, '+' + BROWN);          break;
				}
}

/* draw_map: draw the map background (stuff that is below entities) */
void draw_map_floor(Ent e, int r) {
	for (int i = e.x-r; i < e.x+r && i < MAX_X; i++)
		for (int j = e.y-r; j < e.y+r && j < MAX_Y; j++)
			if (j >= 0)
				switch (get_map(i, j)) {
				case ' ': mvaddch(j, i, get_map(i, j)); break;
				case '.':
					attron(COLOR_PAIR(11));
					mvaddch(j, i,
						(maprand[j][i] == 0)
						? ACS_BULLET : ':');
					attroff(COLOR_PAIR(11));
					break;
				case 'g':
					attron(GRASS);
					mvaddch(j, i,
						(maprand[j][i] == 0)
						? ACS_BULLET : '*');
					attroff(GRASS);
					break;
				case 'w': mvaddch(j, i, '~' + WATER); break;
				case '-': mvaddch(j, i, '-' + BROWN); break;
				}
}

