#include <ncurses.h>
#include <string.h>

#include "ratium.h"
#include "../config.h"

void inv(Ent *e) {

	int arrow_y = 1;
	int k;

	do {
		switch (k) {
			case RAT_UP:
				arrow_y--;
				break;
			case RAT_DOWN:
				arrow_y++;
				break;
			case RAT_GET:
				inv_use_item(e, arrow_y-1);
				break;
			case RAT_DROP:
				inv_drop_item(e, arrow_y-1);
				break;
		}

		if (e->inv[arrow_y-1].face == ' ')
			arrow_y--;

		/* TODO: Make moving arrow work better so this is not needed */
		if (arrow_y <= 0)
			arrow_y = 1;

		draw_inv(e, arrow_y);

	} while ((k = getch()) != RAT_INV);

	clear();

}

void draw_inv(Ent *e, int arrow_y) {

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

void inv_add_item(Ent *e, Item *item, int qty) {
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

void inv_use_item(Ent *e, int num) {
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
void inv_drop_item(Ent *e, int num) {
	if (e->inv[num].qty > 0) {
		for (int i = 0; i < MAX_ITEMS; i++)
			if (e->inv[num].face == item[i].face)
				if (e->inv[num].color == item[i].color)
					add_item(&item[i], e->x, e->y);
		e->inv[num].qty--;
	}
}

void get_item(Ent *e) {
	for (int i = 0; i <= itemqty; i++)
		/* if (item[i].map[e->y][e->x] == item[i].face) { */
		if (item[i].map[e->y][e->x] > 0) {
			inv_add_item(e, &item[i], 1);
			clear_item(&item[i], e->x, e->y);
			return;
		}
	/* TODO: Break into function */
	/* if nothing is under player put what player was holding into inv */
	if (e->holding.face != ' ') {
		e->holding.face = ' ';
		e->holding.color = 0;
		e->holding.type = 0;
		e->holding.stat = 0;
		for (int i = 0; i < MAX_INV_SLOTS; i++)
			if (strcmp(e->inv[i].name, e->holding.name) == 0)
				e->inv[i].qty++;
	}
}

void player_run(int c, Ent *e) {
	if (isalive(e->hp)) {

		switch (c) {
			case RAT_LEFT:
				move_entity(e, -1,  0);
				e->direc = LEFT;
				break;
			case RAT_DOWN:
				move_entity(e,  0,  1);
				e->direc = DOWN;
				break;
			case RAT_UP:
				move_entity(e,  0, -1);
				e->direc = UP;
				break;
			case RAT_RIGHT:
				move_entity(e,  1,  0);
				e->direc = RIGHT;
				break;
			case RAT_STAND: break;
			case RAT_GET:  get_item(e); break;
			case RAT_OPEN: toggle_door(e->x, e->y); break;
			case RAT_INV:  inv(e); break;
		}

	}
}

