/* See LICENSE for licence details. */
#ifndef RATIUM_H
#define RATIUM_H

/* settings for ratium */
#define DEF_PLAYERS 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* max constants */
#define MAX_X 48
#define MAX_Y 32
#define MAX_NAME 16
#define MAX_PLAYERS 8
#define MAX_ENTITIES 256
#define MAX_ITEMS 256
#define MAX_INV 16

#define U 16
#define FONT_W 8
#define FONT_H 16

#undef false /* damn ncurses.... */
#undef true
#undef bool
typedef enum { false, true } bool;

#define SDL_ERROR(x) { \
	printf("SDL Error: %s\n", SDL_GetError()); \
	return x; \
}

/* direction entities can face */
typedef enum {
	LEFT,
	DOWN,
	UP,
	RIGHT,
	LEFTDOWN,
	LEFTUP,
	RIGHTDOWN,
	RIGHTUP
} Direc;

typedef enum {
	BLOCK_NORM,
	BLOCK_DOOR,
	BLOCK_WATER
} BlockType;

typedef enum {
	TEX_NORM,
	TEX_RAND,
	TEX_X,
	TEX_Y,
	TEX_SXY,
	TEX_XY
} BlockTexType;

typedef enum {
	CLEAR_NONE,
	CLEAR_BG,
	CLEAR_FG
} BlockClear;

typedef enum {
	ITEM_MISC,
	ITEM_FOOD,
	ITEM_SWORD,
	ITEM_SHIELD,
	ITEM_SHOOTER,
	ITEM_AMMO
} ItemType;

/* where the entity can spawn */
typedef enum {
	TYPE_ALL,
	TYPE_CAVE,
	TYPE_GRASS,
	TYPE_WATER
} EntType;

/* type of entity ai, or if entity is a player */
typedef enum {
	AI_NONE,
	AI_PLAYER,
	AI_HOSTILE,
	AI_PEACEFUL,
	AI_SHOT
} EntAI;

typedef struct {
	char *name;
	char face;
	BlockType type;
	BlockTexType textype;
	BlockClear texclear;
	bool isfloor;

	SDL_Texture *img;
	SDL_Rect src; /* ent position on sprite sheet */
	/* SDL_Rect dst; /1* ent position *1/ */

	int stat;
} Block;

typedef struct {
	char *name;
	ItemType type;
	int map[MAX_Y][MAX_X]; /* position and count of item in world */
	char face; /* char that gets displayed */

	SDL_Texture *img;
	SDL_Rect src; /* ent position on sprite sheet */
	/* SDL_Rect dst; /1* ent position *1/ */

	int stat;  /* universal variable for damage of sword, heath healed for food, etc. */
} Item;

/* player keys */
typedef struct _Keys Keys;
struct _Keys {
	Uint8 left, down, up, right;
	Uint8 act, drop, inv;
};

/* different armor slots that entities could have items in */
typedef struct _Armor Armor;
struct _Armor {
	/* int which points to ent inventory slot */
	int body, head, chest, hands, legs, feet;
};

typedef struct _Pos Pos;
struct _Pos {
	float x, y, w, h;
};

/* universal struct for players and other entities */
typedef struct _Ent Ent;
struct _Ent {
	char *name;    /* name of ent */
	EntType type; /* used mainly for where to spawn */
	EntAI ai;     /* how ent should move around */

	SDL_Texture *img;
	SDL_Rect src; /* ent position on sprite sheet */
	/* SDL_Rect dst; /1* ent position *1/ */
	int rot;
	SDL_RendererFlip flip;

	Pos pos;
	Direc direc; /* direction ent is facing */

	int maxhp, hp; /* ent heath and max heath */
	bool isdead;   /* set to true after entity death stuff it run */
	int damage;    /* how much damage entity deals to others */
	int sight;     /* how far entity can see */
	float speed;   /* how fast entity ai can move */

	struct _Keys keys; /* player keys */
	char *msg;   /* message player will display, or message to tell player */
	Item inv[MAX_INV]; /* inventory of ent */
	int hand;
	Armor armor;

	void (*run)(Ent *e); /* function containing entity movement logic */
};

/* game.c: game functions */
bool rat_init(void);
void rat_loop(void);
void rat_cleanup(void);

/* map.c: handle the map */
void init_map(void);
char get_map(int x, int y);
void set_map(int x, int y, char newch);
bool is_floor(int x, int y);
bool is_floor_range(int x, int y, int dx, int dy);
int  floor_count(char ch);
void toggle_door(int x, int y);
void draw_map(Ent e, int r);
void draw_map_floor(Ent e, int r);

/* item.c: handle items */
int  query_item(char *name);
void clear_item(Item *item, int x, int y);
void add_item(Item *item, int x, int y);
void draw_item(Item item, Ent e, int r);

/* ent.c: handle anything with entities */
void draw_player_msg(Ent e);
void draw_msg(Ent e);
void draw_ent(Ent e, Ent oe, int r);

/* ai.c: different entity AIs */
void no_ai(Ent *e);
void rand_ai(Ent *e);
void dumb_ai(Ent *e);
void shot_ai(Ent *e);

/* player.c: handle the player */
void draw_inv(Ent e);
void player_run(Ent *e);

/* data.c: handle reading from data/ directory */
bool init_block(void);
bool init_item(void);
bool init_entity(void);
bool init_player(int count);
void init_shot(Pos pos, Direc direc, int dmg, char *ammo);

/* gfx.c: SDL functions */
SDL_Texture *load_img(char *path);
bool draw_text(char *str, SDL_Color color, int x, int y);
void draw_img(SDL_Texture *img, SDL_Rect *src, int x, int y, int rot, SDL_RendererFlip flip);
void draw_img_pos(SDL_Texture *img, SDL_Rect *src, Pos pos, int rot, SDL_RendererFlip flip);
bool pos_collide(Pos pos_1, Pos pos_2);

int ZOOM;
SDL_Window *win;
SDL_Renderer *ren;
TTF_Font *font;

SDL_Texture *nullimg; /* fall back if image file cant be loaded */

int blockqty;
int itemqty;
int playerqty;
int entqty;

Block block[MAX_ITEMS];
Item item[MAX_ITEMS];
Ent player[MAX_PLAYERS]; /* TODO combine player with entity */
Ent entity[MAX_ENTITIES];

#endif /* RATIUM_H */
