/* See LICENSE for licence details. */
#include "ratium.h"
#include "util.h"
#include "rat-lua.h"

/* query_item: return element number of array matching name supplied */
int query_item(char *name) {
	if (name == NULL)
		return -1;
	for (int i = 0; i <= itemqty; i++)
		if (estrcmp(item[i].name, name) == 0)
			return i;
	return -1;
}

/* clear_item: clear item at x and y position */
void clear_item(Item *item, int x, int y) {
	item->map[y][x]--;
}

/* add_item: set item value at x and y position to item face */
void add_item(Item *item, int x, int y) {
	item->map[y][x]++;
}

/* draw_item: draw item if in range of entity e by radius of r */
void draw_item(Item item, Ent e, int r) {
	for (int i = e.pos.x-r; i < e.pos.x+r && i < MAX_X; i++)
		for (int j = e.pos.y-r; j < e.pos.y+r && j < MAX_Y; j++)
			if (item.map[j][i] > 0)
				draw_img(item.img, &item.src, i*U, j*U, 0, SDL_FLIP_NONE);
}

static int
lua_add_item(lua_State *L) {
	LUA_ARG_COUNT(4, "add_item");

	item[itemqty].name = malloc(MAX_NAME * sizeof(char));
	size_t len = MAX_NAME;
	const char *str = lua_tolstring(L, 1, &len);
	strcpy(item[itemqty].name, str);
	item[itemqty].type = lua_tonumber(L, 2);
	item[itemqty].stat = lua_tonumber(L, 3);

	char imgpath[64] = {0};
	sprintf(imgpath, "data/items/%s.png", item[itemqty].name);
	item[itemqty].img = load_img(imgpath);
	item[itemqty].src = (SDL_Rect) { 0, 0, U, U };

	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			item[itemqty].map[j][i] = 0;

	for (int x = 0; x < MAX_X; x++)
		for (int y = 0; y < MAX_Y; y++)
			if (estrcmp(get_map(x, y).name, "stone") == 0 ||
			    estrcmp(get_map(x, y).name, "board") == 0)
				if (rand()%99+1 <= lua_tonumber(L, 4)*100)
					item[itemqty].map[y][x] += (item[itemqty].type == ITEM_AMMO) ? 4 : 1;

	itemqty++;

	return 0;
}

bool
init_item(void) {
	itemqty = 0;

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	LUA_SET_VAR(ITEM_MISC);
	LUA_SET_VAR(ITEM_FOOD);
	LUA_SET_VAR(ITEM_SWORD);
	LUA_SET_VAR(ITEM_SHIELD);
	LUA_SET_VAR(ITEM_SHOOTER);
	LUA_SET_VAR(ITEM_AMMO);
	LUA_LOAD_FILE("data/items.lua");
	lua_register(L, "add_item", lua_add_item);
#include "items.lua.h"
	LUA_LOAD_HEADER("src/items.lua.h", src_items_lua, src_items_lua_len);
	lua_close(L);

	itemqty++;
	return true;
}

