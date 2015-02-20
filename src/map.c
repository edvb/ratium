#include "ratium.h"

char worldMap[MAX_Y][MAX_X+1] = {
"                                                                                ",
"                                                                                ",
"                                                                                ",
"   ############                                              ##############     ",
"   #..........#                                              #ggggggwwwwww#     ",
"   #..........######                                         #ggggggggwwww#     ",
"   #..........#....#                                         #.ggggggggwww#     ",
"   #..........+....#######   #################################..ggggggggww#     ",
"   #####+######....+.....#   #...............................+..gggggggggg#     ",
"       #.#    #....#####.#   #.###############################.g.ggggggggg#     ",
"       #.#    ######   #.#   #.#                             #gggggggggggg#     ",
"       #.#             #.#   #.#                             #gggggggggggg#     ",
"       #.#             #.#   #.#                             #gggggggggggg#     ",
"       #.#           ###+#####+####                          ##############     ",
"    ####+#######     #............#                                             ",
"    #..........#     #............#                                             ",
"    #..........#     ###########+##                                             ",
"    #..........#               #.#                                              ",
"    #########+##               #.#                                              ",
"            #.#                #.#                                              ",
"            #.##################.#                                              ",
"            #....................#                                              ",
"            ######################                                              ",
"                                                                                ",
};

/* get_map: get character of map at x and y position */
char get_map(int x, int y) {
	return worldMap[y][x];
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
				mvaddch(j, i, '#' + A_BOLD);
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
				mvaddch(j, i, '.');
			else if (worldMap[j][i] == 'g')
				mvaddch(j, i, '.' + GRASS);
			else if (worldMap[j][i] == 'w')
				mvaddch(j, i, '~' + WATER);
			else if (worldMap[j][i] == '-')
				mvaddch(j, i, '-' + BROWN);
}

