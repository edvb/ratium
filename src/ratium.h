#ifndef RATIUM_H
#define RATIUM_H

/* some colors, should probably be replaced with enum, or be removed */
#define RED COLOR_PAIR(1)
#define GREEN COLOR_PAIR(2)
#define BLUE COLOR_PAIR(3)
#define YELLOW COLOR_PAIR(4)
#define BROWN COLOR_PAIR(5)
#define GREY COLOR_PAIR(6)
#define DARK_YELLOW COLOR_PAIR(8)
#define WATER COLOR_PAIR(9)
#define GRASS COLOR_PAIR(10)

/* max constants */
#define MAX_X 80
#define MAX_Y 24
#define MAX_NAME 16
#define MAX_PLAYERS 8
#define MAX_ENTITIES 256
#define MAX_ITEMS 256
#define MAX_INV 16

typedef enum { false, true } bool;

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
} DIREC;

typedef enum {
	ITEM_MISC,
	ITEM_FOOD,
	ITEM_SWORD,
	ITEM_SHIELD
} ITEM_TYPE;

/* where the entity can spawn */
typedef enum {
	TYPE_ALL,
	TYPE_CAVE,
	TYPE_GRASS,
	TYPE_WATER
} ENT_TYPE;

/* type of entity ai, or if entity is player */
typedef enum {
	AI_PLAYER,
	AI_HOSTILE,
	AI_PEACEFUL
} ENT_AI;

typedef struct {
	char *name;
	ITEM_TYPE type;
	int map[MAX_Y][MAX_X]; /* position and count of item in world */
	char face; /* char that gets displayed */
	int color; /* color of char */

	int stat;  /* universal variable for damage of sword, heath healed for food, etc. */
} Item;

typedef struct _Msg Msg;
struct _Msg {
	char *data; /* string for contents of message */
	bool disp;  /* if player should display message
		     * or if entity tells message */
};

/* universal struct for players and other entities */
typedef struct _Ent Ent;
struct _Ent {
	char *name;
	ENT_TYPE type; /* used mainly for where to spawn */
	ENT_AI ai;     /* how should it move around */
	char face;     /* char that it displayed */
	int color;     /* color of char */

	DIREC direc; /* direction they are facing */
	int x, y;
	int bary;    /* position of stat bar, mainly for player,
		      * should probably make this auto-assign */

	int maxhp, hp;
	bool isdead; /* set to true after entity death stuff it run */
	int damage;  /* how much damage entity deals to others */
	int sight;   /* how far entity can see */
	int speed;   /* how fast entity ai can move */

	struct _Msg msg; /* message player will display, or message to tell player */
	Item inv[MAX_INV]; /* inventory of entity*/
	/* TODO: change to int */
	Item holding;
};

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

/* entity.c: handle anything with entities */
void draw_ent(Ent e, Ent oe, int r);

/* ai.c: different entity AIs */
void rand_ai(Ent *e, int speed);
void dumb_ai(Ent *e, int xNew, int yNew, int speed);

/* player.c: handle the player */
void add_msg(Msg *msg, char *message);
void draw_msg(Msg *msg);
bool player_run(int c, Ent *e);

/* data.c: handle reading from data/ directory */
bool init_item(void);
bool init_entity(void);
bool init_player(void);

int maxx;
int maxy;

int itemqty;
int playerqty;
int entqty;

Item item[MAX_ITEMS];
Ent player[MAX_PLAYERS];
Ent entity[MAX_ENTITIES]; /* TODO: rename all ent */

#endif /* RATIUM_H */
