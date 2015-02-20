#include "ratium.h"

void inv(entity_t *e) {

	int arrow_y = 1;
	char k;

	do {
		switch (k) {
			case 'j':
				arrow_y++;
				break;
			case 'k':
				arrow_y--;
				break;
			case 'g':
				inv_use_item(e, arrow_y-1);
				break;
			case 'd':
				inv_drop_item(e, arrow_y-1);
				break;
		}

		if (e->inv[arrow_y-1].face == ' ')
			arrow_y--;

		/* TODO: Make moving arrow work better so this is not needed */
		if (arrow_y <= 0)
			arrow_y = 1;

		draw_inv(e, arrow_y);

	} while ((k = getch()) != 'i');

	clear();

}

void draw_inv(entity_t *e, int arrow_y) {

		clear();

		attron(A_REVERSE);
		mvprintw(0, 0, " -- Inventory -- \n");
		attroff(A_REVERSE);

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
		} else if (e->inv[i].name == item->name) {
			e->inv[i].qty += qty;
			return;
		}
}

void inv_use_item(entity_t *e, int num) {
	if (e->inv[num].qty > 0)
		switch (e->inv[num].type) {
			case ITEM_MISC:
				break;
			case ITEM_FOOD:
				e->hp += e->inv[num].stat;
				e->inv[num].qty--;
				break;
			case ITEM_SWORD:
			case ITEM_SHIELD:
				if (e->holding.face == ' ') {
					e->holding.name  = e->inv[num].name;
					e->holding.face  = e->inv[num].face;
					e->holding.color = e->inv[num].color;
					e->holding.type  = e->inv[num].type;
					e->holding.stat  = e->inv[num].stat;
					e->inv[num].qty--;
				}
				break;
		}
}

/* TODO: Allow player to drop multiple items on same tile */
void inv_drop_item(entity_t *e, int num) {
	if (e->inv[num].qty > 0) {
		for (int i = 0; i < MAX_ITEMS; i++)
			if (e->inv[num].face == item[i].face)
				if (e->inv[num].color == item[i].color)
					add_item(&item[i], e->x, e->y);
		e->inv[num].qty--;
	}
}

