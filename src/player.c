/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"
#include "ent.h"
#include "util.h"

static void inv_add_item(Ent *e, Item *item, int qty) {
	for (int i = 0; i < MAX_INV; i++)
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
		while (e->inv[--e->hand].face == ' ' && e->hand != 0) ;
	else if (k[e->keys.right])
		while (e->inv[++e->hand].face == ' ' && e->hand != 0) ;

	if (e->hand >= MAX_INV)
		e->hand = -1;
	else if (e->hand <= -2) {
		e->hand = MAX_INV;
		while (e->inv[--e->hand].face == ' ' && e->hand != 0) ;
	}
}

static void
cmd_run(Ent *e, char *text) {
	char **textv = str_split(text, ' ');

	if (estrcmp(textv[0], "give") == 0) {
		if (query_item(textv[1]) == -1) return;
		for (int i = 0; i < ((textv[2] == NULL) ? 1 : strtol(textv[2], NULL, 10)); i++)
			add_item(&item[query_item(textv[1])], e->pos.x, e->pos.y);
	} else if (estrcmp(textv[0], "heal") == 0)
		e->hp += (textv[1] == NULL) ? e->maxhp : atoi(textv[1]);
	else if (estrcmp(textv[0], "dmg") == 0)
		e->hp -= (textv[1] == NULL) ? 5 : atoi(textv[1]);
	else if (estrcmp(textv[0], "add") == 0) {
		if (textv[1] == NULL) return;
		for (int i = 0; i <= blockqty; i++)
			if (estrcmp(block[i].name, textv[1]) == 0)
				set_map(holding_x(e->direc, e->pos.x), holding_y(e->direc, e->pos.y),
				        block[i]);
	} else if (estrcmp(textv[0], "pos") == 0) {
		e->msg = malloc(MAX_MSG * sizeof(char));
		snprintf(e->msg, MAX_MSG, "%.1f, %.1f", e->pos.x, e->pos.y);
	} else if (estrcmp(textv[0], "spawn") == 0)
		if (textv[1] != NULL)
			add_ent_name(textv[1],
			             holding_x(e->direc, e->pos.x), holding_y(e->direc, e->pos.y),
			             (textv[2] == NULL) ? 1 : atoi(textv[2]));

	free(textv);
}

static void
cmd(Ent *e) {
	bool quit = false;
	SDL_Event event;
	char *text = malloc(sizeof(char*));
	text[0] = '\0';

	SDL_StartTextInput();
	while (!quit) {
		SDL_RenderFillRect(ren, &(SDL_Rect){0,0,MAX_X*U*ZOOM,FONT_H*ZOOM});
		draw_text("/", (SDL_Color){255,255,255}, 0, 0);
		if (strlen(text) > 0)
			draw_text(text, (SDL_Color){255,255,255}, FONT_W, 0);
		SDL_RenderPresent(ren);

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (strlen(text) > 0)
						text[strlen(text)-1] = 0;
					else
						quit = true;
				} else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
					SDL_SetClipboardText(text);
				} else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
					text = SDL_GetClipboardText();
				} else if (event.key.keysym.sym == SDLK_RETURN) {
					if (strlen(text) > 0)
						cmd_run(e, text);
					quit = true;
				}
			} else if (event.type == SDL_TEXTINPUT) {
				if (!((event.text.text[0] == 'c' || event.text.text[0] == 'C') &&
				      (event.text.text[0] == 'v' || event.text.text[0] == 'V') &&
				       SDL_GetModState() & KMOD_CTRL)) {
					char *tmpstr = malloc(strlen(text)+1);
					tmpstr[0] = '\0';
					strcat(tmpstr, text);
					text = malloc(strlen(text)+strlen(event.text.text)+1);
					text[0] = '\0';
					strcat(text, tmpstr);
					strcat(text, event.text.text);
					free(tmpstr);
				}
			}
		}
	}
	SDL_StopTextInput();
	free(text);
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
		for (int i = 0; i < MAX_INV; i++)
			if (e->inv[i].map[0][0] > 0 && e->inv[i].type == ITEM_AMMO) {
				e->inv[i].map[0][0]--;
				e->inv[e->hand].face = ']';
				e->inv[e->hand].src.y = U;
				return;
			}
}

static void
act_key(Ent *e) {
	if (e->t.swing > 0)
		return;
	e->t.swing = 16;

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
			move_entity(e, holding_x(e->direc, 0) / 2, holding_y(e->direc, 0) / 2);
			break;
		case ITEM_SHIELD:
			break;
		}

	for (int i = 0; i <= entqty; i++)
		if (isalive(entity[i].hp) && (pos_collide(e->pos, entity[i].pos) ||
		    pos_collide((Pos){holding_x(e->direc, e->pos.x), holding_y(e->direc, e->pos.y),1,1},
			entity[i].pos)))
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
		else if (k[SDL_SCANCODE_SLASH])  cmd(e);

		if (estrcmp(get_map(e->pos.x, e->pos.y).name, "water") == 0) {
			e->src.h = 8;
			e->pos.h = .5;
		} else {
			e->src.h = U;
			e->pos.h = 1.0;
		}

		/* collect item on ground */
		for (int i = 0; i <= itemqty; i++)
			if (item[i].map[(int)(e->pos.y+.5)][(int)(e->pos.x+.5)] > 0) {
				inv_add_item(e, &item[i], 1);
				clear_item(&item[i], e->pos.x+.5, e->pos.y+.5);
			}

		ent_checks(e); /* TODO: move to main game loop? */

		if (e->msg != NULL) {
			if (e->t.msg != 0) {
				e->t.msg--;
				if (e->t.msg <= 0)
					e->msg = NULL;
			} else
				e->t.msg = 128;
		}

		if (e->t.swing > 0)
			e->t.swing--;

	} else if (!e->isdead) {
		for (int i = 0; i < MAX_INV; i++)
			for (;e->inv[i].map[0][0] > 0; e->inv[i].map[0][0]--)
				add_item(&item[query_item(e->inv[i].name)], e->pos.x, e->pos.y);
		e->msg = "You Died!";
		e->isdead = true;
	}
}

