#include <ncurses.h>

#include "ratium.h"

char worldMap[MAX_Y][MAX_X+1] = {
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

/* get_map: get character of map at x and y position */
char get_map(int x, int y) {
	return worldMap[y][x];
}

/* set_map: set character of map at x and y position to newch */
void set_map(int x, int y, char newch) {
	worldMap[y][x] = newch;
}

bool is_floor(int x, int y) {
	switch (get_map(x, y)) {
		case '#': return false;
		case 'X': return false;
		case 'w': return false;
		case '+': return false;
		default: return true;
	}
}

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
	if (get_map(x-1, y) == '+') set_map(x-1, y, '-');
	else if (get_map(x+1, y) == '+') set_map(x+1, y, '-');
	else if (get_map(x, y-1) == '+') set_map(x, y-1, '-');
	else if (get_map(x, y+1) == '+') set_map(x, y+1, '-');
	else if (get_map(x-1, y) == '-') set_map(x-1, y, '+');
	else if (get_map(x+1, y) == '-') set_map(x+1, y, '+');
	else if (get_map(x, y-1) == '-') set_map(x, y-1, '+');
	else if (get_map(x, y+1) == '-') set_map(x, y+1, '+');
}

/* TODO: Use get_map function */
/* draw_map: draw the map foreground (stuff that is on top of entities) */
void draw_map(Ent e, int r) {
	for (int i = e.x-r; i < e.x+r; i++)
		for (int j = e.y-r; j < e.y+r; j++)
			if (worldMap[j][i] == '#')
				mvaddch(j, i, '#' + COLOR_PAIR(12));
			else if (worldMap[j][i] == 'X')
				mvaddch(j, i, 'X' + COLOR_PAIR(13));
			else if (worldMap[j][i] == '+')
				mvaddch(j, i, '+' + BROWN);
}

/* draw_map: draw the map background (stuff that is below entities) */
void draw_map_floor(Ent e, int r) {
	for (int i = e.x-r; i < e.x+r; i++)
		for (int j = e.y-r; j < e.y+r; j++)
			if (worldMap[j][i] == ' ')
				mvaddch(j, i, worldMap[j][i]);
			else if (worldMap[j][i] == '.')
				mvaddch(j, i, ACS_BULLET + COLOR_PAIR(11));
			else if (worldMap[j][i] == 'g')
				mvaddch(j, i, ACS_BULLET + GRASS);
			else if (worldMap[j][i] == 'w')
				mvaddch(j, i, '~' + WATER);
			else if (worldMap[j][i] == '-')
				mvaddch(j, i, '-' + BROWN);
}

