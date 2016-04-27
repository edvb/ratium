/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"
#include "ent.h"

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

void
draw_inv(Ent e) {
	char s[128] = {0}, hps[3] = {0};
	SDL_Color color = { 255, 255, 255 };
	int x, y;

	sprintf(s, "%s: %d", e.inv[e.hand].name, e.inv[e.hand].map[0][0]);
	sprintf(hps, "%d", e.hp);

	const Uint8 *k = SDL_GetKeyboardState(NULL);
	if (k[e.keys.inv]) {
		if (e.hand != -1 && s != NULL) {
			x = (e.pos.x*U+(U/2)) - (strlen(s)*FONT_W / 2);
			y = (e.pos.y*U-(U/2)) - (FONT_H / 2);
			draw_text(s, color, x, y);
		}
		x = (e.pos.x*U+(U/2)) - (strlen(hps)*FONT_W / 2);
		y = (e.pos.y*U-(U/2)) + (FONT_H *1.5);
		draw_text(hps, color, x, y);
	}
}

static void
inv(Ent *e) {
	const Uint8 *k = SDL_GetKeyboardState(NULL);
	if (k[e->keys.left])
		while (e->inv[++e->hand].face == ' ' && e->hand != 0) ;
	else if (k[e->keys.right])
		while (e->inv[--e->hand].face == ' ' && e->hand != 0) ;

	if (e->hand >= MAX_INV)
		e->hand = -1;
	else if (e->hand <= -2) {
		e->hand = MAX_INV;
		while (e->inv[--e->hand].face == ' ' && e->hand != 0) ;
	}
}

static void
drop_item(Ent *e) {
	if (e->inv[e->hand].map[0][0] > 0) {
		add_item(&item[query_item(e->inv[e->hand].name)],
		         holding_x(e->direc, e->pos.x), holding_y(e->direc, e->pos.y));
		e->inv[e->hand].map[0][0]--;
	}
}

static void
load_shooter(Ent *e) {
	if (e->inv[e->hand].type == ITEM_SHOOTER)
		for (int i = 0; i <= MAX_INV; i++)
			if (e->inv[i].map[0][0] > 0 && e->inv[i].type == ITEM_AMMO) {
				e->inv[i].map[0][0]--;
				e->inv[e->hand].face = ']';
				e->inv[e->hand].src.y = U;
				return;
			}
}

static void
act_key(Ent *e) {
	/* toogle door if looking at one */
	int door_x = holding_x(e->direc, e->pos.x+.5);
	int door_y = holding_y(e->direc, e->pos.y+.5);
	if (get_map(door_x, door_y) == '+' || get_map(door_x, door_y) == '-') {
		toggle_door(door_x, door_y);
		return;
	}

	/* use item in hand */
	if (e->inv[e->hand].map[0][0] > 0)
		switch (e->inv[e->hand].type) {
		case ITEM_MISC:
		case ITEM_AMMO: break;
		case ITEM_FOOD:
			e->hp += e->inv[e->hand].stat;
			e->inv[e->hand].map[0][0]--;
			return;
		case ITEM_SHOOTER:
			if (e->inv[e->hand].face == ']') {
				/* TODO: Make range depened on bow and dmg depened on arrow */
				init_shot(e->pos, e->direc, item[query_item("arrow")].stat, "arrow");
				e->inv[e->hand].face = ')';
				e->inv[e->hand].src.y = 0;
			} else
				load_shooter(e);
			return;
		case ITEM_SWORD:
		case ITEM_SHIELD:
			break;
		}

	for (int i = 0; i <= entqty; i++)
		if (isalive(entity[i].hp) && pos_collide(e->pos, entity[i].pos))
			attack(e, &entity[i]);

}

void
player_run(Ent *e) {
	if (isalive(e->hp)) {
		const Uint8 *k = SDL_GetKeyboardState(NULL);
		if (k[e->keys.left] && !k[e->keys.inv]) {
			move_entity(e, -e->speed, 0);
			e->direc = LEFT;
			e->flip = SDL_FLIP_NONE;
		}
		if (k[e->keys.down] && !k[e->keys.inv]) {
			move_entity(e, 0, e->speed);
			e->direc = DOWN;
		}
		if (k[e->keys.up] && !k[e->keys.inv]) {
			move_entity(e, 0, -e->speed);
			e->direc = UP;
		}
		if (k[e->keys.right] && !k[e->keys.inv]) {
			move_entity(e, e->speed, 0);
			e->direc = RIGHT;
			e->flip = SDL_FLIP_HORIZONTAL;
		}
		if      (k[e->keys.drop]) drop_item(e);
		else if (k[e->keys.act])  act_key(e);
		else if (k[e->keys.inv])  inv(e);

		/* collect item on ground */
		for (int i = 0; i <= itemqty; i++)
			if (item[i].map[(int)(e->pos.y+.5)][(int)(e->pos.x+.5)] > 0) {
				inv_add_item(e, &item[i], 1);
				clear_item(&item[i], e->pos.x+.5, e->pos.y+.5);
			}

		/* make sure heath does not go over max */
		if (e->hp > e->maxhp)
			e->hp = e->maxhp;

	} else if (!e->isdead) {
		for (int i = 0; i < MAX_INV; i++)
			for (;e->inv[i].map[0][0] > 0; e->inv[i].map[0][0]--)
				add_item(&item[query_item(e->inv[i].name)], e->pos.x, e->pos.y);
		e->msg = "You Died!";
		e->isdead = true;
	}
}

