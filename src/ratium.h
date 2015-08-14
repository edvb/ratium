#ifndef RATIUM_H
#define RATIUM_H

#include "../config.h"

/* max constants */
#define MAX_X 80
#define MAX_Y 24
#define MAX_NAME 16
#define MAX_PLAYERS 8
#define MAX_ENTITIES 256
#define MAX_ITEMS 256
#define MAX_INV 16

#undef false /* damn ncurses.... */
#undef true
#undef bool
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
	ITEM_SHOOTER,
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

/* type of entity ai, or if entity is a player */
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

/* player keys */
typedef struct _Keys Keys;
struct _Keys {
	char left, down, up, right;
	char leftdown, leftup, rightdown, rightup;
	char stand, act, drop, inv;
};

typedef struct _Msg Msg;
struct _Msg {
	char *data; /* string for contents of message */
	/* TODO: remove necessity for this by checking if *data is NULL */
	bool disp;  /* if player should display message
		     * or if entity tells message */
};

/* different armor slots that entities could have items in */
typedef struct _Armor Armor;
struct _Armor {
	/* int which points to ent inventory slot */
	int body, head, chest, hands, legs, feet;
};

/* universal struct for players and other entities */
typedef struct _Ent Ent;
struct _Ent {
	char *name;    /* name of ent */
	ENT_TYPE type; /* used mainly for where to spawn */
	ENT_AI ai;     /* how ent should move around */
	char face;     /* the ascii appearance */
	int color;     /* color of char */

	DIREC direc; /* direction ent is facing */
	int x, y;    /* ent position */
	int bary;    /* position of stat bar, mainly for player */

	int maxhp, hp; /* ent heath and max heath */
	bool isdead;   /* set to true after entity death stuff it run */
	int damage;    /* how much damage entity deals to others */
	int sight;     /* how far entity can see */
	int speed;     /* how fast entity ai can move */

	struct _Keys keys; /* player keys */
	struct _Msg msg;   /* message player will display, or message to tell player */
	Item inv[MAX_INV]; /* inventory of ent */
	int hand;
	Armor armor;
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
void rand_ai(Ent *e);
void dumb_ai(Ent *e, int xNew, int yNew);

/* player.c: handle the player */
void add_msg(Msg *msg, char *message);
void draw_msg(Msg *msg);
bool player_run(int c, Ent *e);

/* data.c: handle reading from data/ directory */
bool init_item(void);
bool init_entity(void);
bool init_player(int count);

int maxx;
int maxy;

int itemqty;
int playerqty;
int entqty;

Item item[MAX_ITEMS];
Ent player[MAX_PLAYERS];
Ent entity[MAX_ENTITIES];

#endif /* RATIUM_H */
