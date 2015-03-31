#include <ncurses.h>
#include <stdlib.h>

#include "ratium.h"
#include "ent.h"

/* can_step: determine if entity can move to a new space */
bool can_step(Ent *e, int x, int y) {
	if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
		return false;

	for (int i = 0; i <= entqty; i++)
		if (isalive(entity[i].hp) &&
		    entity[i].x == x && entity[i].y == y) {
			/* TODO: Allow some entities to attack each other */
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
		if (e.type == ENT_PLAYER)
			mvprintw(e.bary, 0, "HP: %d", e.hp);
		mvaddch(e.y, e.x, e.face + e.color);
		if (e.holding.face != ' ')
			mvaddch(holding_y(e, e.y), holding_x(e, e.x),
				e.holding.face + e.holding.color);
	}
}
