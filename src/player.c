#include "ratium.h"

void draw_inv(entity_t *e) {

	int arrow_y = 1;
	char k;

	do {
		clear();

		attron(A_REVERSE);
		mvprintw(0, 0, " -- Inventory -- \n");
		attroff(A_REVERSE);

		switch (k) {
			case 'j':
				if (e->inv[arrow_y].face != ' ')
					arrow_y++;
				break;
			case 'k':
				if (e->inv[arrow_y-1].face != ' ')
					arrow_y--;
				break;
			case 'd':
				for (int i = 0; i < MAX_ITEMS; i++)
					if (e->inv[arrow_y-1].face == item[i].face)
						if (e->inv[arrow_y-1].color == item[i].color)
							add_item(&item[i], e->x, e->y);
				if (e->inv[arrow_y-1].qty > 0)
					e->inv[arrow_y-1].qty--;
				if (e->inv[arrow_y-1].face == ' ')
					arrow_y--;
				break;
			case 'g':
				if (e->inv[arrow_y-1].qty > 0) {
					if (e->inv[arrow_y-1].type == ITEM_FOOD) {
						e->hp += e->inv[arrow_y-1].stat;
						e->inv[arrow_y-1].qty--;
					}
				}
		}

		/* TODO: Make moving arrow work better so this is not needed */
		if (arrow_y <= 0)
			arrow_y = 1;

		for (int i = 0; i < MAX_INV_SLOTS; i++)
			if (e->inv[i].face != ' ') {
				attron(GREY);
				printw("   %c)", i + 97);
				attroff(GREY);

				attron(e->inv[i].color);
				printw(" %c", e->inv[i].face);
				attroff(e->inv[i].color);

				printw(" %s", e->inv[i].name);

				attron(GREY);
				printw(" (%d)\n", e->inv[i].qty);
				attroff(GREY);
			}

		mvprintw(arrow_y, 1, ">");

	} while ((k = getch()) != 'i');

	clear();

	draw_map();

	for (int i = 0; i <= itemCount; i++)
		draw_item(item[i]);

}

void inv_add_item(entity_t *e, item_t *item, int qty) {

	for (int i = 0; i <= MAX_INV_SLOTS; i++)
		if (e->inv[i].face == ' ') {
			e->inv[i].face = item->face;
			e->inv[i].name = item->name;
			e->inv[i].color = item->color;
			e->inv[i].type = item->type;
			e->inv[i].stat = item->stat;
			e->inv[i].qty = qty;
			return;
		} else if (e->inv[i].face == item->face) {
			e->inv[i].qty += qty;
			return;
		}

}

void get_item(entity_t *e) {

	for (int i = 0; i <= itemCount; i++)
		if (item[i].map[e->y][e->x] == item[i].face) {
			inv_add_item(e, &item[i], 1);
			clear_item(&item[i], e->x, e->y);
		}

}

void player_run(char c, entity_t *e) {
	if (e->hp > 0) {

		e->oldx = e->x;
		e->oldy = e->y;

		switch (c) {
			case 'h': move_entity(e, -1,  0); break;
			case 'j': move_entity(e,  0,  1); break;
			case 'k': move_entity(e,  0, -1); break;
			case 'l': move_entity(e,  1,  0); break;
			case 'g': get_item(e); break;
			case 'o': toggle_door(e->x, e->y); break;
			case 'i': draw_inv(e); break;
		}

		for (int i = 1; i < entityCount; i++)
			attack(e, &entity[i]);

		move(e->bary, 0);
		printw("HP: %d", e->hp);
		printw(" (%d, %d)", e->x, e->y);
		mvaddch(e->y, e->x, e->face + e->color);

	}
}
