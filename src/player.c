/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"
#include "ent.h"

void add_msg(char *msg, char *message) {
	strcpy(msg, message);
}

void draw_msg(char *msg) {
	if (msg == NULL)
		return;
	int x, y;
	SDL_Color color = { 255, 255, 255 };
	x = (MAX_X*U / 2) - ((strlen(msg)*FONT_W) / 2);
	y = (MAX_Y*U / 2) - (FONT_H / 2);
	draw_text(msg, color, x, y);
}

static void draw_inv(Ent *e, int arrow_y) {
	char s[50];

	draw_text(" -- Inventory -- ", (SDL_Color){5,5,5}, 8*20, 0);

	for (int i = 0; i < MAX_INV; i++)
		if (e->inv[i].face != ' ') {
			sprintf(s, "   %c) %c %s (%d)",
			        i+97, e->inv[i].face, e->inv[i].name, e->inv[i].map[0][0]);
			draw_text(s, (SDL_Color){5,5,5}, 8*20, (i+1)*40);
		}

	draw_text(">", (SDL_Color){5,5,5}, 1*20, (arrow_y)*40);
}

static void inv_add_item(Ent *e, Item *item, int qty) {
	for (int i = 0; i <= MAX_INV; i++)
		if (e->inv[i].face == ' ') {
			e->inv[i].face = item->face;
			e->inv[i].name = item->name;
			e->inv[i].img  = item->img;
			e->inv[i].src  = item->src;
			e->inv[i].type = item->type;
			e->inv[i].stat = item->stat;
			e->inv[i].map[0][0] = qty;
			return;
		} else if (e->inv[i].name == item->name) {
			e->inv[i].map[0][0] += qty;
			return;
		}
}

static void inv_use_item(Ent *e, int num) {
	if (e->inv[num].map[0][0] > 0)
		switch (e->inv[num].type) {
		case ITEM_MISC:
		case ITEM_AMMO:
			break;
		case ITEM_FOOD:
			e->hp += e->inv[num].stat;
			e->inv[num].map[0][0]--;
			break;
		case ITEM_SHOOTER:
		case ITEM_SWORD:
		case ITEM_SHIELD:
			if (e->hand == -1) {
				e->hand = num;
				e->inv[num].map[0][0]--;
			}
			break;
		}
}

static void inv_drop_item(Ent *e, int num) {
	if (e->inv[num].map[0][0] > 0) {
		for (int i = 0; i < MAX_ITEMS; i++)
			if (e->inv[num].face == item[i].face)
					add_item(&item[i], e->x, e->y);
		e->inv[num].map[0][0]--;
	}
}

static void inv(Ent *e) {
	int arrow_y = 1;

	do {
		const Uint8 *k = SDL_GetKeyboardState(NULL);
		if      (k[e->keys.up])   arrow_y--;
		else if (k[e->keys.down]) arrow_y++;
		else if (k[e->keys.act])  inv_use_item(e, arrow_y-1);
		else if (k[e->keys.drop]) inv_drop_item(e, arrow_y-1);
		else if (k[e->keys.inv])  break;

		if (e->inv[arrow_y-1].name == NULL)
			arrow_y--;
		if (arrow_y <= 0)
			arrow_y = 1;

		draw_inv(e, arrow_y);

	} while (1);

	SDL_RenderClear(ren);

}

static void drop_item(Ent *e) {
	/* if nothing is under player put what player was holding into inv */
	if (e->hand != -1) {
		for (int i = 0; i < MAX_INV; i++)
			if (strcmp(e->inv[i].name, e->inv[e->hand].name) == 0)
				e->inv[i].map[0][0]++;
		e->hand = -1;
	}
}

static bool
fire_spot(int x, int y, int dmg) {
	for (int i = 0; i < entqty; i++)
		if (entity[i].x == x && entity[i].y == y) {
			entity[i].hp -= dmg;
			return false;
		}
	if (!is_floor(x, y))
		return false;
	/* rat_mvaddch(x, y, 'x', 1); */

	return true;
}

static void
fire_shooter(DIREC direc, int x_0, int y_0, int range, int dmg) {
	/* int i = x_0, j = y_0; */
	switch (direc) {
	case LEFT:
		for (int i = x_0; i > x_0-range; i--)
			if (!fire_spot(i, y_0, dmg)) return;
		break;
	case DOWN:
		for (int i = y_0; i < y_0+range; i++)
			if (!fire_spot(x_0, i, dmg)) return;
		break;
	case UP:
		for (int i = y_0; i > y_0-range; i--)
			if (!fire_spot(x_0, i, dmg)) return;
		break;
	case RIGHT:
		for (int i = x_0; i < x_0+range; i++)
			if (!fire_spot(i, y_0, dmg)) return;
		break;
	case LEFTDOWN:
		for (int i = x_0, j = y_0; i > x_0-range && j < y_0+range; i--, j++)
			if (!fire_spot(i, j, dmg)) return;
		break;
	case LEFTUP:
		for (int i = x_0, j = y_0; i > x_0-range && j > y_0-range; i--, j--)
			if (!fire_spot(i, j, dmg)) return;
		break;
	case RIGHTDOWN:
		for (int i = x_0, j = y_0; i < x_0+range && j > y_0-range; i++, j++)
			if (!fire_spot(i, j, dmg)) return;
		break;
	case RIGHTUP:
		for (int i = x_0, j = y_0; i < x_0+range && j > y_0-range; i++, j--)
			if (!fire_spot(i, j, dmg)) return;
		break;
	}
}

static void
load_shooter(Ent *e) {
	if (e->inv[e->hand].type == ITEM_SHOOTER)
		for (int i = 0; i <= MAX_INV; i++)
			if (e->inv[i].map[0][0] > 0 &&
			    e->inv[i].type == ITEM_AMMO) {
				e->inv[i].map[0][0]--;
				e->inv[e->hand].face = ']';
				return;
			}
}

static void act_key(Ent *e) {
	/* toogle door if looking at one */
	int door_x = holding_x(*e, e->x);
	int door_y = holding_y(*e, e->y);
	int dmg;
	if (get_map(door_x, door_y) == '+' || get_map(door_x, door_y) == '-') {
		toggle_door(door_x, door_y);
		return;
	}

	/* use item in hand */
	switch (e->inv[e->hand].type) {
	case ITEM_MISC:
	case ITEM_AMMO:
	case ITEM_FOOD: break;
	case ITEM_SHOOTER:
		if (e->inv[e->hand].face == ']') {
			/* TODO: Make range depened on bow and dmg depened on arrow */
			fire_shooter(e->direc, e->x, e->y, 20, 5);
			e->inv[e->hand].face = ')';
		} else
			load_shooter(e);
		break;
	case ITEM_SWORD:
	case ITEM_SHIELD:
		break;
	}

}

bool player_run(Ent *e) {
	if (isalive(e->hp)) {
		bool returnval = true; /* true if key pressed is valid key */

		const Uint8 *k = SDL_GetKeyboardState(NULL);
		if (k[e->keys.left] && !k[e->keys.inv]) {
			e->msg = NULL;
			move_entity(e, -1, 0);
			e->direc = LEFT;
			e->flip = SDL_FLIP_NONE;
		}
		if (k[e->keys.down] && !k[e->keys.inv]) {
			e->msg = NULL;
			move_entity(e, 0, 1);
			e->direc = DOWN;
		}
		if (k[e->keys.up] && !k[e->keys.inv]) {
			e->msg = NULL;
			move_entity(e, 0, -1);
			e->direc = UP;
		}
		if (k[e->keys.right] && !k[e->keys.inv]) {
			e->msg = NULL;
			move_entity(e, 1, 0);
			e->direc = RIGHT;
			e->flip = SDL_FLIP_HORIZONTAL;
		}
		if      (k[e->keys.drop]) drop_item(e);
		else if (k[e->keys.act])  act_key(e);
		else if (k[e->keys.inv]) { /* TODO: move into inv() */
			if (k[e->keys.left])
				while (e->inv[++e->hand].face == ' ' && e->hand != 0) ;
			else if (k[e->keys.right])
				while (e->inv[--e->hand].face == ' ' && e->hand != 0) ;
			if (e->hand >= MAX_INV) e->hand = -1;
			else if (e->hand <= -2) {
				e->hand = MAX_INV;
				while (e->inv[--e->hand].face == ' ' && e->hand != 0) ;
			}
		}

		/* collect item on ground */
		for (int i = 0; i <= itemqty; i++)
			if (item[i].map[e->y][e->x] > 0) {
				inv_add_item(e, &item[i], 1);
				clear_item(&item[i], e->x, e->y);
			}

		/* make sure heath does not go over max */
		if (e->hp > e->maxhp)
			e->hp = e->maxhp;

		return returnval;

	} else if (!e->isdead) {
		/* TODO: Improve death, drop items on ground */
		/* add_msg(player[0].msg, "You Died!"); */
		e->msg = "You Died!";
		e->isdead = true;
		return true;
	} else
		return false;
}

