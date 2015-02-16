#include "ratium.h"

int query_item(char *name) {
	for (int i = 0; i <= itemqty; i++)
		if (strcmp(item[i].name, name) == 0)
			return i;
}

/* TODO: Move to map file */
void toggle_door(int x, int y) {
	if (get_map(x-1, y) == '+') set_map_char(x-1, y, '-');
	else if (get_map(x+1, y) == '+') set_map_char(x+1, y, '-');
	else if (get_map(x, y-1) == '+') set_map_char(x, y-1, '-');
	else if (get_map(x, y+1) == '+') set_map_char(x, y+1, '-');
	else if (get_map(x-1, y) == '-') set_map_char(x-1, y, '+');
	else if (get_map(x+1, y) == '-') set_map_char(x+1, y, '+');
	else if (get_map(x, y-1) == '-') set_map_char(x, y-1, '+');
	else if (get_map(x, y+1) == '-') set_map_char(x, y+1, '+');
}

void clear_item(item_t *item, int x, int y) {
	item->map[y][x] = ' ';
}

void add_item(item_t *item, int x, int y) {
	item->map[y][x] = item->face;
}

void add_item_ch(item_t *item, int x, int y, char newch) {
	item->map[y][x] = newch;
}

void draw_item(item_t item, entity_t e, int r) {
	for (int i = e.x-r; i < e.x+r; i++)
		for (int j = e.y-r; j < e.y+r; j++)
			if (item.map[j][i] != ' ')
				mvaddch(j, i, item.map[j][i] + item.color);
}

