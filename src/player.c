#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"
#include "ent.h"
#include "../config.h"

void add_msg(Msg *msg, char *message) {
	strcpy(msg->data, message);
	msg->disp = true;
}

void draw_msg(Msg *msg) {
	if (msg->disp == true) {
		int x, y;
		x = (maxx / 2) - (strlen(msg->data) / 2);
		y = maxy/2;
		attron(COLOR_PAIR(12));
		mvprintw(y, x, msg->data);
		attroff(COLOR_PAIR(12));
		msg->disp = false;
	}
}

static void draw_inv(Ent *e, int arrow_y) {

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

static void inv_add_item(Ent *e, Item *item, int qty) {
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

static void inv_use_item(Ent *e, int num) {
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

static void inv_drop_item(Ent *e, int num) {
	if (e->inv[num].qty > 0) {
		for (int i = 0; i < MAX_ITEMS; i++)
			if (e->inv[num].face == item[i].face)
				if (e->inv[num].color == item[i].color)
					add_item(&item[i], e->x, e->y);
		e->inv[num].qty--;
	}
}

static void inv(Ent *e) {
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

static void get_item(Ent *e) {
	for (int i = 0; i <= itemqty; i++)
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

bool player_run(int c, Ent *e) {
	if (isalive(e->hp)) {
		bool returnval; /* true if key pressed is valid key */

		switch (c) {
		case '4':
		case RAT_LEFT:
			move_entity(e, -1, 0);
			e->direc = LEFT;
			returnval = true;
			break;
		case '2':
		case RAT_DOWN:
			move_entity(e, 0, 1);
			e->direc = DOWN;
			returnval = true;
			break;
		case '8':
		case RAT_UP:
			move_entity(e, 0, -1);
			e->direc = UP;
			returnval = true;
			break;
		case '6':
		case RAT_RIGHT:
			move_entity(e, 1, 0);
			e->direc = RIGHT;
			returnval = true;
			break;
		case '1':
		case RAT_LEFTDOWN:
			move_entity(e, -1, 1);
			e->direc = LEFTDOWN;
			returnval = true;
			break;
		case '7':
		case RAT_LEFTUP:
			move_entity(e, -1, -1);
			e->direc = LEFTUP;
			returnval = true;
			break;
		case '3':
		case RAT_RIGHTDOWN:
			move_entity(e, 1, 1);
			e->direc = RIGHTDOWN;
			returnval = true;
			break;
		case '9':
		case RAT_RIGHTUP:
			move_entity(e, 1, -1);
			e->direc = RIGHTUP;
			returnval = true;
			break;
		case '5':
		case RAT_STAND: returnval = true; break;
		case RAT_GET:   get_item(e); returnval = true; break;
		case RAT_OPEN:  toggle_door(e->x, e->y); returnval = true; break;
		case RAT_INV:   inv(e); returnval = true; break;
		default: returnval = false; break;
		}

		if (e->hp > e->maxhp)
			e->hp = e->maxhp;

		return returnval;

	} else
		return false;
}

