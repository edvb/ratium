Block block_t[MAX_ITEMS] = {
{ "../null",     0,           0,        0,          0,           1,     0, {0}, 0  },
{ "grass",       BLOCK_NORM,  TEX_RAND, CLEAR_NONE, SPAWN_ALL,   true,  0, {0}, 4  },
{ "dirt",        BLOCK_NORM,  TEX_XY,   CLEAR_BG,   SPAWN_ALL,   true,  0, {0}, 0  },
{ "rock",        BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   false, 0, {0}, 0  },
{ "stone",       BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   true,  0, {0}, 0  },
{ "plank",       BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   false, 0, {0}, 0  },
{ "board",       BLOCK_NORM,  TEX_NORM, CLEAR_NONE, SPAWN_ALL,   true,  0, {0}, 0  },
{ "water",       BLOCK_WATER, TEX_SXY,  CLEAR_BG,   SPAWN_ALL,   true,  0, {0}, 0  },
{ "door",        BLOCK_DOOR,  TEX_NORM, CLEAR_FG,   SPAWN_ALL,   false, 0, {0}, 0  },
{ "window",      BLOCK_NORM,  TEX_NORM, CLEAR_FG,   SPAWN_ALL,   false, 0, {0}, 0  },
{ "chair",       BLOCK_NORM,  TEX_NORM, CLEAR_BG,   SPAWN_ALL,   true,  0, {0}, 0  },
{ "table",       BLOCK_NORM,  TEX_X,    CLEAR_BG,   SPAWN_ALL,   false, 0, {0}, 0  },
{ "barrel",      BLOCK_NORM,  TEX_NORM, CLEAR_BG,   SPAWN_GRASS, false, 0, {0}, 0  },
{ "bush",        BLOCK_NORM,  TEX_RAND, CLEAR_BG,   SPAWN_GRASS, false, 0, {0}, 16 },
{ "flower",      BLOCK_NORM,  TEX_RAND, CLEAR_BG,   SPAWN_GRASS, true,  0, {0}, 4  },
{ "tall_grass",  BLOCK_NORM,  TEX_NORM, CLEAR_BG,   SPAWN_GRASS, true,  0, {0}, 0  },
};
int blockqty_t = 16;

Block map[MAX_Y][MAX_X];

typedef struct _Map Map;
struct _Map {
	size_t len, height;
	int rarity;
	int map[MAX_Y][MAX_X];
};

struct _Map buildings[10] = {
{ 8, 6, 2, {
{0,7,7,7,7,7,0,0},
{7,7,7,7,7,7,7,0},
{7,7,7,7,7,7,7,7},
{0,0,7,7,7,7,7,7},
{0,0,0,7,7,7,7,0},
{0,0,0,0,7,7,0,0} }},
{ 8, 6, 2, {
{0,0,0,7,7,7,0,0},
{0,0,7,7,7,7,7,0},
{0,7,7,7,7,7,7,0},
{7,7,7,7,7,7,0,0},
{7,7,7,7,7,7,0,0},
{0,7,7,0,0,0,0,0} }},
{ 9, 9, 1, {
{5, 5,  9,  5,  5, 9,  9,  5,  5},
{5, 10, 11, 10, 6, 6,  6,  6,  5},
{5, 6,  6,  6,  6, 11, 11, 11, 5},
{5, 10, 11, 10, 6, 6,  6,  6,  5},
{5, 6,  6,  6,  6, 6,  6,  12, 5},
{5, 10, 11, 10, 6, 6,  6,  12, 5},
{5, 5,  9,  9,  5, 8,  5,  5,  5},
{0, 10, 0,  10, 2, 2,  2,  2,  2},
{0, 0,  0,  0,  0, 2,  2,  0,  0},
}},
{ 10, 10, 1, {
{5, 5,  9,  5,  9,  5,  9,  5,  5,  5},
{5, 6,  6,  6,  6,  6,  6,  6,  6,  5},
{5, 11, 11, 11, 11, 11, 11, 11, 6,  5},
{5, 10, 10, 10, 10, 10, 10, 10, 6,  5},
{5, 10, 6,  6,  6,  6,  6,  6,  6,  5},
{5, 11, 6,  6,  10, 11, 10, 6,  10, 5},
{5, 10, 6,  6,  6,  6,  6,  10, 11, 5},
{5, 5,  9,  8,  5,  9,  9,  5,  5,  5},
{0, 2,  2,  2,  2,  2,  0,  10, 0,  10},
{0, 0,  0,  2,  2,  0,  0,  0,  0,  0},
}},
};

