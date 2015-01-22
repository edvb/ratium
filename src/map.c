#include "ratium.h"

char worldMap[24][81] = {
"                                                                                ",
"                                                                                ",
"                                                                                ",
"   ############                                              ##############     ",
"   #..........#                                              #ggggggwwwwww#     ",
"   #..........######                                         #ggggggggwwww#     ",
"   #..........#....#                                         #.ggggggggwww#     ",
"   #...............#######   #################################..ggggggggww#     ",
"   #####.######..........#   #..................................gggggggggg#     ",
"       #.#    #....#####.#   #.###############################.g.ggggggggg#     ",
"       #.#    ######   #.#   #.#                             #gggggggggggg#     ",
"       #.#             #.#   #.#                             #gggggggggggg#     ",
"       #.#             #.#   #.#                             #gggggggggggg#     ",
"       #.#           ###.#####.####                          ##############     ",
"    ####.#######     #............#                                             ",
"    #..........#     #............#                                             ",
"    #..........#     ###########.##                                             ",
"    #..........#               #.#                                              ",
"    #########.##               #.#                                              ",
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

void draw_map() {

	for (int j = 0; j < 24; j++) {
		for (int i = 0; i < 80; i++) {
			if (worldMap[j][i] == ' ') {
				mvaddch(j, i, worldMap[j][i]);
			} else if (worldMap[j][i] == '#') {
				mvaddch(j, i, '#' + A_BOLD);
			} else if (worldMap[j][i] == '.') {
				mvaddch(j, i, '.');
			} else if (worldMap[j][i] == 'g') {
				mvaddch(j, i, '.' + GRASS);
			} else if (worldMap[j][i] == 'w') {
				mvaddch(j, i, '~' + WATER);
			} else {
				mvaddch(j, i, worldMap[j][i]);
			}
		}
	}

}
