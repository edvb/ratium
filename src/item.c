#include "ratium.h"

void init_item(item_t item) {
	for (int j = 0; j < 24; j++)
		for (int i = 0; i < 80; i++)
			item.map[j][i] = ' ';
}

void draw_item(item_t item) {
	for (int j = 0; j < 24; j++)
		for (int i = 0; i < 80; i++)
			if (item.map[j][i] == item.face)
				mvaddch(j, i, item.face + item.color);
}
