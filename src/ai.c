/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"
#include "ent.h"

static bool
check_isalive(Ent *e) {
	if (!isalive(e->hp)) {
		if (!e->isdead) { /* TODO: Remove isdead var, use hp var at -1 instead */
			e->isdead = true;
			for (int i = 0; i < MAX_INV; i++)
				for (;e->inv[i].map[0][0] > 0; e->inv[i].map[0][0]--)
					add_item(&item[query_item(e->inv[i].name)], e->pos.x, e->pos.y);
		}
		return false;
	} else
		return true;
}

static void
attack_player(Ent *e) {
	for (int i = 0; i <= playerqty; i++)
		if (isalive(player[i].hp) && pos_collide(e->pos, player[i].pos))
			attack(e, &player[i]);
}

void
no_ai(Ent *e) {
	if (!check_isalive(e)) return;
	attack_player(e);

	ent_checks(e);
}

void
rand_ai(Ent *e) {
	if (!check_isalive(e)) return;

	for (int i = 0; i <= playerqty; i++)
		if (e->msg != NULL && pos_collide(e->pos, player[i].pos)) {
			ent_checks(e);
			return;
		}

	int direc = rand() % (int)e->speed;

	switch (direc) {
	case 0: move_entity(e, -1,  0); break;
	case 1: move_entity(e,  0,  1); break;
	case 2: move_entity(e,  0, -1); break;
	case 3: move_entity(e,  1,  0); break;
	}

	attack_player(e);

	ent_checks(e);
}

void
dumb_ai(Ent *e) {
	if (!check_isalive(e)) return;

	for (int i = 0; i <= playerqty; i++)
		if (e->msg != NULL && pos_collide(e->pos, player[i].pos)) {
			ent_checks(e);
			return;
		}

	int shouldMove;
	if (abs(e->pos.x - player[0].pos.x) <= e->sight && /* TODO: Make dumb AI follow other players */
	    abs(e->pos.y - player[0].pos.y) <= e->sight) {

		if ((shouldMove = rand() % (int)e->speed) != 0)
			return;

		if (player[0].pos.x > e->pos.x)
			move_entity(e, 1, 0);
		else if (player[0].pos.x < e->pos.x)
			move_entity(e, -1, 0);
		if (player[0].pos.y > e->pos.y)
			move_entity(e, 0, 1);
		else if (player[0].pos.y < e->pos.y)
			move_entity(e, 0, -1);

		attack_player(e);

		ent_checks(e);

	} else
		rand_ai(e);
}

void
shot_ai(Ent *e) {
	if (!isalive(e->hp))
		return;
	e->pos.x += holding_x(e->direc, 0)*e->speed;
	e->pos.y += holding_y(e->direc, 0)*e->speed;
	for (int i = 0; i <= entqty; i++)
		if (pos_collide(e->pos, entity[i].pos) && entity[i].ai != AI_SHOT) {
			attack(e, &entity[i]);
			e->hp = 0;
		}
	if (!can_step(e->pos))
		e->hp = 0;
}
