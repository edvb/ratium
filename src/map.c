/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"
#include "map.h"
#include "util.h"

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
place_block(Block b) {
	int x, y, tries = 0;
	do {
		x = rand() % MAX_X;
		y = rand() % MAX_Y;
		if (tries > 100000) return;
		tries++;
	} while (is_spawn(x, y, b.spawntype));
	set_map((int)x, (int)y, b);
}

static void /* puts a cluster of blocks on map */
place_blocks(Block b) {
	int x_0, y_0, qty, pos;
	int tries = 0;

	do {
		x_0 = rand() % MAX_X;
		y_0 = rand() % MAX_Y;
		if (tries > 100000) return;
		tries++;
	} while (is_spawn_range(x_0-1, y_0-1, 3, 3, b.spawntype));
	set_map(x_0, y_0, b);

	qty = rand()%8;
	for (int j = 0; j < qty; j++) {
		pos = rand()%8;
		switch (pos) {
		case 0: set_map(x_0+1, y_0, b); break;
		case 1: set_map(x_0, y_0+1, b); break;
		case 2: set_map(x_0+1, y_0+1, b); break;
		case 3: set_map(x_0-1, y_0, b); break;
		case 4: set_map(x_0, y_0-1, b); break;
		case 5: set_map(x_0-1, y_0-1, b); break;
		case 6: set_map(x_0+1, y_0-1, b); break;
		case 7: set_map(x_0-1, y_0+1, b); break;
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
				if (bld.map[j][i] != 0)
					set_map(i+x_0, j+y_0, block[bld.map[j][i]]);
	}
}

static void /* place a rectangle of wall filled with floor on map */
place_room(Block wall, Block floor, int doorqty, Block door) {
	int x_0 = 0, y_0 = 0;
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

bool
init_block(void) {
	blockqty = 0;

	for (int num = 0; num < blockqty_t; num++) {
		block[blockqty] = block_t[blockqty];

		block[blockqty].name = malloc(MAX_NAME * sizeof(char));
		strcpy(block[blockqty].name, block_t[blockqty].name);

		char imgpath[64] = {0};
		sprintf(imgpath, "data/blocks/%s.png", block[blockqty].name);
		block[blockqty].img = load_img(imgpath);
		block[blockqty].src = (SDL_Rect) { 0, 0, U, U };

		blockqty++;
	}

	return true;
}

void /* draw buildings and rooms to map */
init_map(void) {
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			set_map(i, j, block[1]);
	for (int i = 0; i < 10; i++) /* create buildings */
		place_bld(buildings[i]);
	for (int i = 0; i < rand()%6; i++) /* create houses */
		place_room(block[5], block[6], rand()%2+1, block[8]);
	for (int i = 0; i < rand()%4+3; i++) /* create dungeons */
		place_room(block[3], block[4], rand()%3+1, block[8]);
	for (int i = 0; i < rand()%2+1; i++) {
		place_blocks(block[12]); /* create barrels */
		place_blocks(block[15]); /* create tall grass patches */
	}
	for (int i = 0; i < rand()%8+8; i++) {
		place_block(block[13]);  /* create bushes */
		place_blocks(block[14]); /* create flowers */
	}
}

char * /* return tile which SpawnType can spawn on */
spawn_tile(SpawnType type) {
	switch(type) {
	case SPAWN_ALL:   return NULL;
	case SPAWN_CAVE:  return "stone";
	case SPAWN_GRASS: return "grass";
	case SPAWN_WATER: return "water";
	default:          return NULL;
	}
}

bool /* return true if x and y can not spawn there */
is_spawn(int x, int y, SpawnType type) {
	return (type == SPAWN_ALL) ? !is_floor(x, y) : (estrcmp(get_map(x, y).name, spawn_tile(type)) != 0);
}

bool /* return true if x and y can not spawn in range */
is_spawn_range(int x, int y, int dx, int dy, SpawnType type) {
	if (type == SPAWN_ALL)
		return !is_floor_range(x, y, dx, dy);
	else
		for (int i = x; i <= dx+x; i++)
			for (int j = y; j <= dy+y; j++)
				if (estrcmp(get_map(i, j).name, spawn_tile(type)) != 0)
					return true;
	return false;
}

Block /* get character of map at x and y position */
get_map(int x, int y) {
	if (x > MAX_X || y >= MAX_Y || x < 0 || y < 0)
		return block[0];
	else
		return map[y][x];
}

void /* set character of map at x and y position to newch */
set_map(int x, int y, Block b) {
	if (x > MAX_X || y >= MAX_Y || x < 0 || y < 0) ;
	else
		map[y][x] = b;
}

bool /* returns true if tile at x and y is a floor tile */
is_floor(int x, int y) {
	return get_map(x, y).isfloor;
}

bool /* return whether or not the area given is all a floor tile */
is_floor_range(int x, int y, int dx, int dy) {
	for (int i = x; i <= dx+x; i++)
		for (int j = y; j <= dy+y; j++)
			if (estrcmp(get_map(i, j).name, "grass") != 0) /* TODO: is_floor */
				return false;
	return true;
}

/* floor_count: return how many tiles of charater ch are on map */
int floor_count(char ch) {
	int count = 0;
	/* for (int i = 0; i < MAX_X; i++) */
	/* 	for (int j = 0; j < MAX_Y; j++) */
	/* 		if (ch == '*') { */
	/* 			if (is_floor(i, j)) count++; */
	/* 		} else { */
	/* 			if (get_map(i, j) == ch) count++; */
	/* 		} */
	return count;
}

/* toggle_door: open or close door next to entity */
void toggle_door(int x, int y) {
	if (get_map(x,y).type != BLOCK_DOOR)
		return;
	map[y][x].stat = (get_map(x,y).stat == 0) ? 1 : 0;
	map[y][x].isfloor = (get_map(x,y).stat == 0) ? false : true;
}

/* TODO: clean up connect code */
static void /* modify a block's src Rect depending on neighboring blocks to connect them */
src_connect(Block b, int x, int y, SDL_Rect *src) {
	if (b.type == BLOCK_DOOR) {
		src->x = (get_map(x,y).stat == 0) ? 0 : U;
		return;
	}
	*src = (SDL_Rect){0,0,U,U};
	bool above = estrcmp(get_map(x, y-1).name, b.name) == 0;
	bool below = estrcmp(get_map(x, y+1).name, b.name) == 0;
	bool left  = estrcmp(get_map(x-1, y).name, b.name) == 0;
	bool right = estrcmp(get_map(x+1, y).name, b.name) == 0;
	bool upleft    = estrcmp(get_map(x-1, y-1).name, b.name) == 0;
	bool upright   = estrcmp(get_map(x+1, y-1).name, b.name) == 0;
	bool downleft  = estrcmp(get_map(x-1, y+1).name, b.name) == 0;
	bool downright = estrcmp(get_map(x+1, y+1).name, b.name) == 0;
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
				if (((b.texclear == CLEAR_BG) ? get_map(i,j).isfloor : !get_map(i,j).isfloor) &&
				    get_map(i,j).texclear == CLEAR_NONE) {
					draw_img(get_map(i,j).img, &(SDL_Rect){0,0,U,U}, x*U, y*U, 0, SDL_FLIP_NONE);
					return;
			}
}

/* draw_map: draw the map */
void draw_map(Ent e, int r) {
	for (int i = e.pos.x-r; i < e.pos.x+r && i < MAX_X; i++)
		for (int j = e.pos.y-r; j < e.pos.y+r && j < MAX_Y; j++)
			if (i <= MAX_X && j <= MAX_Y && i >= 0 && j >= 0) {
				for (int num = 0; num < blockqty; num++)
					if (estrcmp(block[num].name, get_map(i,j).name) == 0) {
						draw_clear_bg(block[num], i ,j);
						src_connect(block[num], i, j, &block[num].src);
						draw_img(block[num].img, &block[num].src, i*U, j*U, 0, SDL_FLIP_NONE);
					}
			}
}

