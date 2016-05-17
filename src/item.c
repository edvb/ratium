/* See LICENSE for licence details. */
#include <string.h>

#include "ratium.h"

/* query_item: return element number of array matching name supplied */
int query_item(char *name) {
	if (name == NULL)
		return -1;
	for (int i = 0; i <= itemqty; i++)
		if (strcmp(item[i].name, name) == 0)
			return i;
	return -1;
}

/* clear_item: clear item at x and y position */
void clear_item(Item *item, int x, int y) {
	item->map[y][x]--;
}

/* add_item: set item value at x and y position to item face */
void add_item(Item *item, int x, int y) {
	item->map[y][x]++;
}

/* draw_item: draw item if in range of entity e by radius of r */
void draw_item(Item item, Ent e, int r) {
	for (int i = e.pos.x-r; i < e.pos.x+r && i < MAX_X; i++)
		for (int j = e.pos.y-r; j < e.pos.y+r && j < MAX_Y; j++)
			if (item.map[j][i] > 0)
				draw_img(item.img, &item.src, i*U, j*U, 0, SDL_FLIP_NONE);
}

