/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>

#include "ratium.h"
#include "ent.h"

/* can_step: determine if entity can move to a new space */
bool can_step(Pos pos) {
	if (pos.x < 0 || pos.x+pos.w > MAX_X || pos.y < 0 || pos.y+pos.h > MAX_Y)
		return false;

	if (!is_floor(pos.x, pos.y) ||
	    !is_floor(pos.x+pos.w-.000001, pos.y+pos.h-.000001) ||
	    !is_floor(pos.x, pos.y+pos.h-.000001) ||
	    !is_floor(pos.x+pos.w-.000001, pos.y))
		return false;

	return true;
}

/* move_entity: move a entity by dx and dy */
void move_entity(Ent *e, float dx, float dy) {
	if (can_step((Pos){e->pos.x+dx,e->pos.y+dy,e->pos.w,e->pos.h})) {
		e->pos.x += dx;
		e->pos.y += dy;
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
float holding_x(Direc direc, float val) {
	switch (direc) {
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
float holding_y(Direc direc, float val) {
	switch (direc) {
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

void
draw_player_msg(Ent e) {
	if (e.msg == NULL)
		return;
	SDL_Color color = { 255, 255, 255 };
	int x = (e.pos.x*U+(U/2)) - (strlen(e.msg)*FONT_W / 2);
	int y = (e.pos.y*U-(U/2)) - (FONT_H / 2);
	draw_text(e.msg, color, x, y);
}

void
draw_msg(Ent e) {
	if (!isalive(e.hp) || e.msg == NULL)
		return;
	for (int i = 0; i <= playerqty; i++)
		if (pos_collide(e.pos, player[i].pos)) {
			SDL_Color color = { 255, 255, 255 };
			int x = (e.pos.x*U+(U/2)) - (strlen(e.msg)*FONT_W / 2);
			int y = (e.pos.y*U-(U/2)) - (FONT_H / 2);
			draw_text(e.msg, color, x, y);
		}
}

/* draw_ent: draw entity e if in range of entity oe by r */
void draw_ent(Ent e, Ent oe, int r) {
	if (isalive(e.hp) &&
	    oe.pos.x-r < e.pos.x && oe.pos.x+r > e.pos.x &&
	    oe.pos.y-r < e.pos.y && oe.pos.y+r > e.pos.y) {
		SDL_Rect dsthand = {
			((holding_x(e.direc, e.pos.x)*U)-U/2*holding_x(e.direc, 0)+2)*ZOOM,
			((e.pos.y*U)+4)*ZOOM,
			U*.8*ZOOM, U*.8*ZOOM
		};
		draw_img(e.img, &e.src, e.pos.x*U, e.pos.y*U, e.rot, e.flip);
		if (e.hand != -1)
			SDL_RenderCopyEx(ren, e.inv[e.hand].img, &e.inv[e.hand].src, &dsthand, 0, NULL, e.flip);
	}
}
