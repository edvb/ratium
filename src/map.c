/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"
#include "rat-lua.h"
#include "map.h"
#include "util.h"

bool
init_block(void) {
	blockqty = 0;

	for (int num = 0; num < blockqty_t; num++) {
		block[blockqty] = block_t[blockqty];

		block[blockqty].name = emalloc(MAX_NAME * sizeof(char));
		strcpy(block[blockqty].name, block_t[blockqty].name);

		block[blockqty].img = load_img(get_data("gfx/blocks/%s.png", block[blockqty].name));
		block[blockqty].src = (SDL_Rect) { 0, 0, U, U };

		blockqty++;
	}

	return true;
}

static int
lua_is_spawn(lua_State *L) {
	LUA_ARG_COUNT(3, "is_spawn");
	return !is_spawn(lua_tonumber(L, 1), lua_tonumber(L, 2),
	                 block[(int)lua_tonumber(L, 3)].spawntype);
}

static int
lua_is_spawn_range(lua_State *L) {
	LUA_ARG_COUNT(5, "is_spawn_range");
	return !is_spawn_range(lua_tonumber(L, 1), lua_tonumber(L, 2),
	                       lua_tonumber(L, 3), lua_tonumber(L, 4),
	                       block[(int)lua_tonumber(L, 5)].spawntype);
}

static int
lua_set_map(lua_State *L) {
	LUA_ARG_COUNT(3, "set_map");
	set_map(lua_tonumber(L, 1), lua_tonumber(L, 2), block[(int)lua_tonumber(L, 3)]);
	return 0;
}

void /* draw buildings and rooms to map */
init_map(void) {
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			set_map(i, j, block[1]);
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "set_map", lua_set_map);
	lua_register(L, "is_spawn", lua_is_spawn);
	lua_register(L, "is_spawn_range", lua_is_spawn_range);
	LUA_SET_VAR(MAX_X);
	LUA_SET_VAR(MAX_Y);
	LUA_LOAD_FILE(get_data("data/blds.lua"));
	LUA_LOAD_FILE(get_data("data/map.lua"));
	lua_close(L);
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
	if (x >= MAX_X || y >= MAX_Y || x < 0 || y < 0)
		return block[0];
	else
		return map[y][x];
}

void /* set character of map at x and y position to newch */
set_map(int x, int y, Block b) {
	if (x < MAX_X && y < MAX_Y && x >= 0 && y >= 0)
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
			if (!is_floor(i,j))
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

void /* open or close door */
toggle_door(int x, int y) {
	if (get_map(x,y).type != BLOCK_DOOR)
		return;
	map[y][x].stat = (get_map(x,y).stat == 0) ? 1 : 0;
	map[y][x].isfloor = (get_map(x,y).stat == 0) ? false : true;
}

void /* force door open or close */
set_door(int x, int y, bool isopen) {
	if (get_map(x,y).type != BLOCK_DOOR)
		return;
	map[y][x].stat = isopen;
	map[y][x].isfloor = isopen;
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
			if (i < MAX_X && j < MAX_Y && i >= 0 && j >= 0) {
				for (int num = 0; num < blockqty; num++)
					if (estrcmp(block[num].name, get_map(i,j).name) == 0) {
						draw_clear_bg(block[num], i ,j);
						src_connect(block[num], i, j, &block[num].src);
						draw_img(block[num].img, &block[num].src, i*U, j*U, 0, SDL_FLIP_NONE);
					}
			}
}

