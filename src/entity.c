#include <ncurses.h>
#include <stdlib.h>

#include "ratium.h"

/* can_step: determine if entity can move to a new space */
bool can_step(Ent *e, int x, int y) {
	if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
		return false;

	for (int i = 0; i <= entqty; i++)
		/* TODO: Check if holding item */
		if (isalive(entity[i].hp) &&
		    entity[i].x == x && entity[i].y == y) {
			if (e->type == ENT_PLAYER) {
				if (entity[i].msg.disp == true)
					add_msg(&player[0].msg, entity[i].msg.data);
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

/* move_entity: move a entity by x_0 and y_0 */
void move_entity(Ent *e, int dx, int dy) {
	if (can_step(e, e->x + dx, e->y + dy)) {
		e->x += dx;
		e->y += dy;
	}
}

/* attack: entity e attack entity foe */
void attack(Ent *e, Ent *foe) {
	if (e->type != ENT_PEACEFUL)
		take_damage(foe, deal_damage(e));
}

void take_damage(Ent *e, int damage) {
	switch (e->holding.type) {
		case ITEM_SHIELD:
			e->holding.stat -= damage;
			if (e->holding.stat < 0) {
				e->holding.face = ' ';
				e->holding.color = 0;
				e->holding.type = 0;
				e->holding.stat = 0;
			}
			break;
		default:
			e->hp -= damage;
	}
}

int deal_damage(Ent *e) {
	switch (e->holding.type) {
		case ITEM_SWORD:
			return e->damage + e->holding.stat;
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
		case LEFT:  return val-1;
		case RIGHT: return val+1;
		default:    return val;
	}
}

/* holding: return y position for what entity is holding */
int holding_y(Ent e, int val) {
	switch (e.direc) {
		case DOWN:  return val+1;
		case UP:    return val-1;
		default:    return val;
	}
}

/* draw_ent: draw entity e if in range of entity oe by r */
void draw_ent(Ent e, Ent oe, int r) {
	if (isalive(e.hp) &&
	    oe.x-r < e.x && oe.x+r > e.x &&
	    oe.y-r < e.y && oe.y+r > e.y) {
		if (e.face == '@')
			mvprintw(e.bary, 0, "HP: %d", e.hp);
		mvaddch(e.y, e.x, e.face + e.color);
		if (e.holding.face != ' ')
			mvaddch(holding_y(e, e.y), holding_x(e, e.x),
				e.holding.face + e.holding.color);
	}
}

/* TODO: Move to ai.c file */
/* TODO: Fix Speed */
void rand_ai(Ent *e, int speed) {
	if (isalive(e->hp)) {

		int direc = rand() % speed;

		switch (direc) {
			case 0: move_entity(e, -1,  0); break;
			case 1: move_entity(e,  0,  1); break;
			case 2: move_entity(e,  0, -1); break;
			case 3: move_entity(e,  1,  0); break;
		}

	} else if (!e->isdead) {
		e->isdead = true;
		add_item(&item[query_item(e->drop)], e->x, e->y);
	}
}

/* TODO: Make entity have sight and move randomly when player not in sight */
void dumb_ai(Ent *e, int xNew, int yNew, int speed) {
	if (isalive(e->hp)) {

		int shouldMove = rand() % speed;

		if (shouldMove != 0) {
			if (xNew > e->x)
				move_entity(e, 1, 0);
			else if (xNew < e->x)
				move_entity(e, -1, 0);
			if (yNew > e->y)
				move_entity(e, 0, 1);
			else if (yNew < e->y)
				move_entity(e, 0, -1);
		}

	} else if (!e->isdead) {
		e->isdead = true;
		add_item(&item[query_item(e->drop)], e->x, e->y);
	}
}

