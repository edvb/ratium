#include "ratium.h"

void init_item(item_t *item) {
	for (int j = 0; j < 24; j++)
		for (int i = 0; i < 80; i++)
			item->map[j][i] = ' ';
}

void clear_item(item_t *item, int x, int y) {
	item->map[y][x] = ' ';
}

void draw_item(item_t item) {
	for (int j = 0; j < 24; j++)
		for (int i = 0; i < 80; i++)
			if (item.map[j][i] != ' ')
				mvaddch(j, i, item.map[j][i] + item.color);
}
