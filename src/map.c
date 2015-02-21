#include "ratium.h"

char worldMap[MAX_Y][MAX_X+1] = {
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggwwwwwgggggg",
"ggg############gggggggggggggggggggggggggggggggggggggggggggggggggggggwwwwwwwggggg",
"ggg#..........#gggggggggggggggggggggggggggggggggggggggggggggggggggggwwwwwwwwgggg",
"ggg#..........######ggggggggggggggggggggggggggggggggggggggggggggggggggwwwwwwgggg",
"ggg#..........#....#ggggggggggggggggggggggggggggggggggggggggg..ggggggggwwwwggggg",
"ggg#..........+....#######ggg#################################..ggggggggwwgggggg",
"ggg#####+######....+.....#ggg#...............................+..gggggggggggggggg",
"ggggggg#.#gggg#....#####.#ggg#.###############################.g.ggggggggggggggg",
"ggggggg#.#gggg######ggg#.#ggg#.#gggggggggggggggggggggggggggg...ggggggggggggggggg",
"ggggggg#.#ggggggggggggg#.#ggg#.#ggggggggggggggggggggggggggggg.gggggggggggggggggg",
"ggggggg#.#ggggggggggggg#.#ggg#.#gggggggggggggggggggggggggggggggggggggggggggggggg",
"ggggggg#.#ggggggggggg###+#####+####ggggggggggggggggggggggggggggggggggggggggggggg",
"gggg####+#######ggggg#............#ggggggggggggggggggggggggggggggggggggggggggggg",
"gggg#..........#ggggg#............#ggggggggggggggggggggggggggggggggggggggggggggg",
"gggg#..........#ggggg###########+##gggggggggggXXXXXXXX+XXXgggggggggggggggggggggg",
"gggg#..........#ggggggggggggggg#.#ggggggggggggX..........Xgggggggggggggggggggggg",
"gggg#########+##ggggggggggggggg#.#ggggggggggggX..........Xgggggggggggggggggggggg",
"gggggggggggg#.#gggggggggggggggg#.#ggggggggggggX..........Xgggggggggggggggggggggg",
"gggggggggggg#.##################.#ggggggggggggXXXXXXXXXXXXgggggggggggggggggggggg",
"gggggggggggg#....................#gggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggg######################gggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",
};

/* get_map: get character of map at x and y position */
char get_map(int x, int y) {
	return worldMap[y][x];
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

/* set_map: set character of map at x and y position to newch */
void set_map(int x, int y, char newch) {
	worldMap[y][x] = newch;
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

/* draw_map: draw the map foreground (stuff that is on top of entities) */
void draw_map(entity_t e, int r) {
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
void draw_map_floor(entity_t e, int r) {
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

