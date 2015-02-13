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

char get_map(int x, int y) {

	char ch;
	ch = worldMap[y][x];

	return ch;
}

void set_map(int x, int y) {
	worldMap[y][x] = ' ';
}

void set_map_char(int x, int y, char newch) {
	worldMap[y][x] = newch;
}

void draw_map(entity_t e, int r) {
	for (int i = e.x-r; i < e.x+r; i++)
		for (int j = e.y-r; j < e.y+r; j++)
			if (worldMap[j][i] == ' ')
				mvaddch(j, i, worldMap[j][i]);
			else if (worldMap[j][i] == '#')
				mvaddch(j, i, '#' + A_BOLD);
			else if (worldMap[j][i] == '.')
				mvaddch(j, i, '.');
			else if (worldMap[j][i] == 'g')
				mvaddch(j, i, '.' + GRASS);
			else if (worldMap[j][i] == 'w')
				mvaddch(j, i, '~' + WATER);
			else if (worldMap[j][i] == '+')
				mvaddch(j, i, '+' + BROWN);
			else if (worldMap[j][i] == '-')
				mvaddch(j, i, '-' + BROWN);
			else
				mvaddch(j, i, worldMap[j][i]);
}

