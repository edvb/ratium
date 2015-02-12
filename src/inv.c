#include "ratium.h"

void draw_inv(entity_t *e) {

	int arrow_y = 1;
	char k;

	do {
		clear();

		attron(A_REVERSE);
		mvprintw(0, 0, " -- Inventory -- \n");
		attroff(A_REVERSE);

		/* TODO: Clean up this pile of puke code */
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
					switch (e->inv[arrow_y-1].type) {
						case ITEM_MISC:
							break;
						case ITEM_FOOD:
							e->hp += e->inv[arrow_y-1].stat;
							e->inv[arrow_y-1].qty--;
							break;
						case ITEM_SWORD:
							e->damage += e->inv[arrow_y-1].stat;
							e->inv[arrow_y-1].qty--;
							break;
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

