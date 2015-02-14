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

	itemqty = to;
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

