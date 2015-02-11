#include "ratium.h"

void alloc_item(int from, int to) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char face;
	int color;
	item_type type;
	int stat;

	FILE *f = fopen("data/items.txt", "r");

	for (int num = from; num <= to; num++) {
		fscanf(f, "%s %c %i %i %i\n", name, &face, &color, &type, &stat);

		item[num].name = malloc(MAX_NAME * sizeof(char));
		strcpy(item[num].name, name);
		item[num].face = face;
		item[num].color = COLOR_PAIR(color);
		item[num].type = type;
		item[num].stat = stat;

		for (int i = 0; i < MAX_X; i++)
			for (int j = 0; j < MAX_Y; j++)
				item[num].map[j][i] = ' ';

		for (int x, y, i = 0; i < 5; i++) {
			do {
				x = rand() % MAX_X;
				y = rand() % MAX_Y;
			} while (get_map(x, y) != '.');
			item[num].map[y][x] = item[num].face;
		}

	}

	fclose(f);
	free(name);

	itemCount = to;
}

void toggle_door(int x, int y) {
	if (get_map(y-1, x) == '+') set_map_char(y-1, x, '-');
	else if (get_map(y+1, x) == '+') set_map_char(y+1, x, '-');
	else if (get_map(y, x-1) == '+') set_map_char(y, x-1, '-');
	else if (get_map(y, x+1) == '+') set_map_char(y, x+1, '-');
	else if (get_map(y-1, x) == '-') set_map_char(y-1, x, '+');
	else if (get_map(y+1, x) == '-') set_map_char(y+1, x, '+');
	else if (get_map(y, x-1) == '-') set_map_char(y, x-1, '+');
	else if (get_map(y, x+1) == '-') set_map_char(y, x+1, '+');

	draw_map();
	for (int i = 0; i <= itemCount; i++)
		draw_item(item[i]);
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

void draw_item(item_t item) {
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (item.map[j][i] != ' ')
				mvaddch(j, i, item.map[j][i] + item.color);
}

