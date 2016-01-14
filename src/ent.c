/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>

#include "ratium.h"
#include "gfx.h"
#include "ent.h"

/* can_step: determine if entity can move to a new space */
bool can_step(Ent *e, int x, int y) {
	if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
		return false;

	for (int i = 0; i <= entqty; i++)
		if (isalive(entity[i].hp) &&
		    entity[i].x == x && entity[i].y == y) {
			if (e->ai == AI_PLAYER) {
				if (entity[i].msg != NULL &&
				    e->inv[e->hand].type != ITEM_SWORD)
					add_msg(player[0].msg, entity[i].msg);
				else
					attack(e, &entity[i]);
			}
			return false;
		}
	for (int i = 0; i <= playerqty; i++)
		if (isalive(entity[i].hp) &&
		    player[i].x == x && player[i].y == y) {
			attack(e, &player[i]);
			return false;
		}

	if (!is_floor(x, y))
		return false;

	return true;
}

/* move_entity: move a entity by dx and dy */
void move_entity(Ent *e, int dx, int dy) {
	if (can_step(e, e->x + dx, e->y + dy)) {
		e->x += dx;
		e->y += dy;
	}
}

/* attack: entity e attack entity foe */
void attack(Ent *e, Ent *foe) {
	if (e->ai != AI_PEACEFUL)
		take_damage(foe, deal_damage(e));
}

/* take_damage: determine how much damage entity e should take */
void take_damage(Ent *e, int damage) {
	switch (e->inv[e->hand].type) {
		case ITEM_SHIELD:
			e->inv[e->hand].stat -= damage;
			if (e->inv[e->hand].stat < 0) {
				e->hand = -1;
			}
			break;
		default:
			e->hp -= damage;
	}
}

/* deal_damage: determine how much damage entity e should give to other entities */
int deal_damage(Ent *e) {
	switch (e->inv[e->hand].type) {
		case ITEM_SWORD:
			return e->damage + e->inv[e->hand].stat;
			break;
		default:
			return e->damage;
	}
}

/* isalive: determine if entity is alive */
bool isalive(int hp) {
	return (hp > 0) ? true : false;
}

/* holding: return x position for what entity is holding */
int holding_x(Ent e, int val) {
	switch (e.direc) {
		case LEFT:
		case LEFTDOWN:
		case LEFTUP:
			return val-1;
		case RIGHT:
		case RIGHTDOWN:
		case RIGHTUP:
			return val+1;
		default: return val;
	}
}

/* holding: return y position for what entity is holding */
int holding_y(Ent e, int val) {
	switch (e.direc) {
		case DOWN:
		case LEFTDOWN:
		case RIGHTDOWN:
			return val+1;
		case UP:
		case LEFTUP:
		case RIGHTUP:
			return val-1;
		default: return val;
	}
}

/* draw_ent: draw entity e if in range of entity oe by r */
void draw_ent(Ent e, Ent oe, int r) {
	if (isalive(e.hp) &&
	    oe.x-r < e.x && oe.x+r > e.x &&
	    oe.y-r < e.y && oe.y+r > e.y) {
		if (e.ai == AI_PLAYER) {
			char str[MAX_NAME];
			sprintf(str, "HP: %d", e.hp);
			rat_mvprint(0, e.bary, str, 0);
		}
		rat_mvaddch(e.x, e.y, e.face, e.color);
		if (e.hand != -1)
			rat_mvaddch(holding_x(e, e.x), holding_y(e, e.y),
			            e.inv[e.hand].face, e.inv[e.hand].color);
	}
}

