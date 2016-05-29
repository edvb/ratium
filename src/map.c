/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"
#include "map.h"

static bool /* determines postion for a building */
find_bld_loc(int *x_0, int *y_0, int w, int h) {
	int tries = 0;
	while (is_spawn_range(*x_0-1, *y_0-1, w+1, h+1, SPAWN_GRASS)) { /* TODO: add type var */
		*x_0 = rand() % MAX_X;
		*y_0 = rand() % MAX_Y;
		if (tries > 100000)
			return false;
		tries++;
	}
	return true;
}

static void /* puts a single block on map */
place_block(int id) {
	int x, y;
	do {
		x = rand() % MAX_X;
		y = rand() % MAX_Y;
	} while (is_spawn(x, y, block[id].spawntype));
	set_map((int)x, (int)y, block[id].face);
}

static void /* puts a cluster of blocks on map */
place_blocks(int id) {
	int x_0, y_0;
	int qty;
	int pos;

	do {
		x_0 = rand() % MAX_X;
		y_0 = rand() % MAX_Y;
	} while (is_spawn_range(x_0-1, y_0-1, 3, 3, block[id].spawntype));
	set_map(x_0, y_0, block[id].face);

	qty = rand()%8;
	for (int j = 0; j < qty; j++) {
		pos = rand()%8;
		switch (pos) {
		case 0: set_map(x_0+1, y_0, block[id].face); break;
		case 1: set_map(x_0, y_0+1, block[id].face); break;
		case 2: set_map(x_0+1, y_0+1, block[id].face); break;
		case 3: set_map(x_0-1, y_0, block[id].face); break;
		case 4: set_map(x_0, y_0-1, block[id].face); break;
		case 5: set_map(x_0-1, y_0-1, block[id].face); break;
		case 6: set_map(x_0+1, y_0-1, block[id].face); break;
		case 7: set_map(x_0-1, y_0+1, block[id].face); break;
		}
	}
}

static void /* puts building bld onto the map */
place_bld(Map bld) {
	int x_0, y_0;

	switch (rand() % 2) { /* randomly change amount of buildings */
	case 0: bld.rarity += rand() % 2;
	case 1: bld.rarity -= rand() % 2;
	}

	for (int num = 0; num < bld.rarity; num++) {
		if (!find_bld_loc(&x_0, &y_0, bld.len, bld.height))
			return;
		for (int i = 0; i < bld.len; i++) /* copy building to world */
			for (int j = 0; j < bld.height; j++)
				if (bld.map[j][i] != ' ')
					set_map(i+x_0, j+y_0, bld.map[j][i]);
	}
}

static void /* place a rectangle of wall filled with floor on map */
place_room(char wall, char floor, int doorqty, char door) {
	int x_0, y_0;
	int len = rand() % 7 + 4;
	int height = rand() % 7 + 4;

	if (!find_bld_loc(&x_0, &y_0, len, height))
		return;

	for (int i = 0; i < len; i++)
		for (int j = 0; j < height; j++)
			set_map(i+x_0, j+y_0, wall);
	for (int i = 0; i < len-2; i++)
		for (int j = 0; j < height-2; j++)
			set_map(i+x_0+1, j+y_0+1, floor);
	for (int i = 0; i < doorqty; i++)
		switch (rand()%4) {
		case 0:
			set_map(x_0+rand()%(len-2)+1, y_0, door);
			break;
		case 1:
			set_map(x_0, y_0+rand()%(height-2)+1, door);
			break;
		case 2:
			set_map(x_0+rand()%(len-2)+1, y_0+height-1, door);
			break;
		case 3:
			set_map(x_0+len-1, y_0+rand()%(height-2)+1, door);
			break;
		}
}

void /* draw buildings and rooms to map */
init_map(void) {
	memset(worldMap, 'g', MAX_Y*MAX_X);
	for (int i = 0; i < 10; i++) /* create buildings */
		place_bld(buildings[i]);
	for (int i = 0; i < rand()%6; i++) /* create houses */
		place_room('X', 'b', rand()%2+1, '+');
	for (int i = 0; i < rand()%4+3; i++) /* create dungeons */
		place_room('#', '.', rand()%3+1, '+');
	for (int i = 0; i < rand()%2+1; i++) {
		place_blocks(12); /* create barrels */
		place_blocks(15); /* create tall grass patches */
	}
	for (int i = 0; i < rand()%8+8; i++) {
		place_block(13);  /* create bushes */
		place_blocks(14); /* create flowers */
	}
}

char /* return tile which SpawnType can spawn on */
spawn_tile(SpawnType type) {
	switch(type) {
	case SPAWN_ALL:   return ' ';
	case SPAWN_CAVE:  return '.';
	case SPAWN_GRASS: return 'g';
	case SPAWN_WATER: return 'w';
	default:          return ' ';
	}
}

bool /* return true if x and y can not spawn there */
is_spawn(int x, int y, SpawnType type) {
	int tile = spawn_tile(type);
	return (type == SPAWN_ALL) ? !is_floor(x, y) : get_map(x, y) != tile;
}

bool /* return true if x and y can not spawn in range */
is_spawn_range(int x, int y, int dx, int dy, SpawnType type) {
	int tile = spawn_tile(type);
	if (type == SPAWN_ALL)
		return !is_floor_range(x, y, dx, dy);
	else
		for (int i = x; i <= dx+x; i++)
			for (int j = y; j <= dy+y; j++)
				if (get_map(i, j) != tile)
					return true;
	return false;
}

/* get_map: get character of map at x and y position */
char get_map(int x, int y) {
	if (x > MAX_X || y > MAX_Y)
		return ' ';
	else
		return worldMap[y][x];
}

/* set_map: set character of map at x and y position to newch */
void set_map(int x, int y, char newch) {
	worldMap[y][x] = newch;
}

bool /* returns true if tile at x and y is a floor tile */
is_floor(int x, int y) {
	for (int i = 0; i < blockqty; i++)
		if (get_map(x, y) == block[i].face)
			return block[i].isfloor;
	return true;
}

bool /* return whether or not the area given is all a floor tile */
is_floor_range(int x, int y, int dx, int dy) {
	for (int i = x; i <= dx+x; i++)
		for (int j = y; j <= dy+y; j++)
			if (get_map(i, j) != 'g') /* TODO: is_floor */
				return false;
	return true;
}

/* floor_count: return how many tiles of charater ch are on map */
int floor_count(char ch) {
	int count;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (ch == '*') {
				if (is_floor(i, j)) count++;
			} else {
				if (get_map(i, j) == ch) count++;
			}
	return count;
}

/* toggle_door: open or close door next to entity */
void toggle_door(int x, int y) {
	if      (get_map(x, y) == '+') set_map(x, y, '-');
	else if (get_map(x, y) == '-') set_map(x, y, '+');
}

/* TODO: clean up connect code */
static void /* modify a block's src Rect depending on neighboring blocks to connect them */
src_connect(Block b, int x, int y, SDL_Rect *src) {
	*src = (SDL_Rect){0,0,U,U};
	bool above = get_map(x, y-1) == b.face;
	bool below = get_map(x, y+1) == b.face;
	bool left  = get_map(x-1, y) == b.face;
	bool right = get_map(x+1, y) == b.face;
	bool upleft    = get_map(x-1, y-1) == b.face;
	bool upright   = get_map(x+1, y-1) == b.face;
	bool downleft  = get_map(x-1, y+1) == b.face;
	bool downright = get_map(x+1, y+1) == b.face;
	switch (b.textype) {
	case TEX_NORM: break;
	case TEX_RAND:
		src->x = (x+y*x)%b.stat*U;
		break;
	case TEX_X:
		if (left && right)
			src->x = 1*U;
		else if (right)
			src->x = 0*U;
		else if (left)
			src->x = 2*U;
		else
			src->x = 3*U;
		break;
	case TEX_Y:
		if (above && below)
			src->y = 1*U;
		else if (below)
			src->y = 0*U;
		else if (above)
			src->y = 2*U;
		else
			src->y = 3*U;
		break;
	case TEX_SXY:
		if (above && below && left && right) {
			if (!upleft) {
				src->x = 1*U;
				src->y = 2*U;
			} else if (!upright) {
				src->x = 0*U;
				src->y = 2*U;
			} else if (!downleft) {
				src->x = 1*U;
				src->y = 1*U;
			} else if (!downright) {
				src->x = 0*U;
				src->y = 1*U;
			}
		} else if (!above && below && left && right) {
			src->x = 3*U;
			src->y = 0*U;
		} else if (above && !below && left && right) {
			src->x = 3*U;
			src->y = 2*U;
		} else if (above && below && !left && right) {
			src->x = 2*U;
			src->y = 1*U;
		} else if (above && below && left && !right) {
			src->x = 4*U;
			src->y = 1*U;
		} else if (!above && below && !left && right) {
			src->x = 2*U;
			src->y = 0*U;
		} else if (!above && below && left && !right) {
			src->x = 4*U;
			src->y = 0*U;
		} else if (above && !below && !left && right) {
			src->x = 2*U;
			src->y = 2*U;
		} else if (above && !below && left && !right) {
			src->x = 4*U;
			src->y = 2*U;
		}
		break;
	case TEX_XY:
		if (above) src->x += 1*U;
		if (left)  src->x += 2*U;
		if (below) src->x += 4*U;
		if (right) src->x += 8*U;
		if (above && left)
			if (upleft)
				src->y = U;
		if (above && right)
			if (upright)
				src->y = U;
		if (below && left)
			if (downleft)
				src->y = U;
		if (below && right)
			if (downright)
				src->y = U;
		if (above && left && below && right) {
			if (!upleft && !upright && !downleft && !downright)
				src->y = U;
			if (!upleft && upright && downleft && downright) {
				src->x = 5*U;
				src->y = 2*U;
			} else if (upleft && !upright && downleft && downright) {
				src->x = 4*U;
				src->y = 2*U;
			} else if (upleft && upright && !downleft && downright) {
				src->x = 5*U;
				src->y = 1*U;
			} else if (upleft && upright && downleft && !downright) {
				src->x = 4*U;
				src->y = 1*U;
			}
		}
		break;
	}
}

static void
draw_clear_bg(Block b, int x, int y) {
	if (b.texclear == CLEAR_NONE)
		return;
	for (int i = x-1; i <= x+1; i++)
		for (int j = y-1; j <= y+1; j++)
			for (int num = 0; num <= blockqty; num++)
				if (get_map(i, j) == block[num].face &&
				    ((b.texclear == CLEAR_BG) ? block[num].isfloor : !block[num].isfloor) &&
				    block[num].texclear == CLEAR_NONE && block[num].type == BLOCK_NORM) {
					draw_img(block[num].img, &(SDL_Rect){0,0,U,U}, x*U, y*U, 0, SDL_FLIP_NONE);
					return;
			}
}

/* draw_map: draw the map */
void draw_map(Ent e, int r) {
	SDL_Rect src;
	for (int i = e.pos.x-r; i < e.pos.x+r && i < MAX_X; i++)
		for (int j = e.pos.y-r; j < e.pos.y+r && j < MAX_Y; j++)
			if (j >= 0) {
				for (int num = 0; num <= blockqty; num++)
					if (get_map(i, j) == block[num].face) {
						draw_clear_bg(block[num], i ,j);
						src_connect(block[num], i, j, &src);
						draw_img(block[num].img, &src, i*U, j*U, 0, SDL_FLIP_NONE);
					}
			}
}

