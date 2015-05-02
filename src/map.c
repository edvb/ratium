#include <ncurses.h>
#include <stdlib.h>

#include "ratium.h"

static char worldMap[MAX_Y][MAX_X+1] = {
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggg###################ggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggg#.................######gggggggggggggggggggwwwwwgggggg",
"ggg#####++#####ggggggggggg#.................+....#ggggggggggggggggggwwwwwwwggggg",
"ggg#..........#ggggggggggg##############+####....#ggggggggggggggggggwwwwwwwwgggg",
"ggg#..........######ggggggggggggggggggg#.#gg######ggggggggggggggggggggwwwwwwgggg",
"ggg#..........#....#ggggggggggggggggggg#.#gggggggggggggg..gggggggggggggwwwwggggg",
"ggg#..........+....#######ggg###########.################..gggggggggggggwwgggggg",
"ggg#####+######..........#ggg#..........................+..ggggggggggggggggggggg",
"ggggggg#.#gggg#....#####.#ggg#.#################+########.g.gggggggggggggggggggg",
"ggggggg#.#gggg######ggg#.#ggg#.#gggggggggggggg#...#gggg...gggggggggggggggggggggg",
"ggggggg+.+ggggggggggggg#.#ggg#.#gggggggggggggg#...#ggggg.ggggggggggggggggggggggg",
"ggggggg#.#ggggggggggggg#.#ggg#.#gggggggggggggg#####ggggggggggggggggggggggggggggg",
"ggggggg#.#ggggggggggg###+#####+####ggggggggggggggggggggggggggggggggggggggggggggg",
"gggg####.#######ggggg#............#gggggggggggggggggggggggggggggggggggggggggggg#",
"gggg#..........#ggggg#............#gggggggggggggggggggggggggggggggggggggggg#####",
"gggg#..........#ggggg###########+##gggggggggggXXXXXXXX+XXXgggggggggggggg########",
"gggg+..........#ggggggggggggggg#.#ggggggggggggX..........Xggggggggggg###########",
"gggg#########+##ggggggggggggggg+.#ggggggggggggX..........Xggggggggg#############",
"gggggggggggg#.#gggggggggggggggg#.#ggggggggggggX..........Xgggggggg##############",
"gggggggggggg#.##################.#ggggggggggggXXXXXXXXXXXXggggggg###############",
"gggggggggggg#....................#ggggggggggggggggggggggggggggggg###############",
"gggggggggggg######################gggggggggggggggggggggggggggggg################",
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg################",
};

static int maprand[MAX_Y][MAX_X+1];

/* init_map: asign values to maprand to determine if character displayed there
 * should be a different char */
void init_map(void) {
	int num;
	for (int i = 0; i < MAX_X; i++)
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

/* floor_count: return how many tiles of charater ch are on map */
int floor_count(char ch) {
	int count;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			/* TODO: Incorporate smart is_floor intergration */
			if (get_map(i, j) == ch)
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

