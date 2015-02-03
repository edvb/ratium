#include "ratium.h"

void alloc_item(item_t *item) {
	for (int j = 0; j < 24; j++)
		for (int i = 0; i < 80; i++)
			item->map[j][i] = ' ';
}

void toggle_door(int x, int y) {
	if (door.map[y-1][x] == '+')
		door.map[y-1][x] = '-';
	else if (door.map[y+1][x] == '+')
		door.map[y+1][x] = '-';
	else if (door.map[y][x-1] == '+')
		door.map[y][x-1] = '-';
	else if (door.map[y][x+1] == '+')
		door.map[y][x+1] = '-';
	else if (door.map[y-1][x] == '-')
		door.map[y-1][x] = '+';
	else if (door.map[y+1][x] == '-')
		door.map[y+1][x] = '+';
	else if (door.map[y][x-1] == '-')
		door.map[y][x-1] = '+';
	else if (door.map[y][x+1] == '-')
		door.map[y][x+1] = '+';
	draw_item(door);
}

/* TODO: Add add_item function */
void clear_item(item_t *item, int x, int y) {
	item->map[y][x] = ' ';
}

void add_item(item_t *item, int x, int y, char newch) {
	item->map[y][x] = newch;
}

void draw_item(item_t item) {
	for (int j = 0; j < 24; j++)
		for (int i = 0; i < 80; i++)
			if (item.map[j][i] != ' ')
				mvaddch(j, i, item.map[j][i] + item.color);
}
