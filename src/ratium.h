#ifndef RATIUM_H
#define RATIUM_H

#define ever (;;)

#define RED COLOR_PAIR(1)
#define GREEN COLOR_PAIR(2)
#define BLUE COLOR_PAIR(3)
#define YELLOW COLOR_PAIR(4)
#define BROWN COLOR_PAIR(5)
#define GREY COLOR_PAIR(6)
#define DARK_YELLOW COLOR_PAIR(8)
#define WATER COLOR_PAIR(9)
#define GRASS COLOR_PAIR(10)

#define MAX_X 80
#define MAX_Y 24
#define MAX_NAME 16
#define MAX_PLAYERS 8
#define MAX_ENTITIES 256
#define MAX_ITEMS 256
#define MAX_INV 16

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

typedef enum {
	TYPE_ALL,
	TYPE_CAVE,
	TYPE_GRASS,
	TYPE_WATER
} ENT_TYPE;

typedef enum {
	AI_PLAYER,
	AI_HOSTILE,
	AI_PEACEFUL
} ENT_AI;

typedef struct {
	char *name;
	ITEM_TYPE type;
	int map[MAX_Y][MAX_X];
	char face;
	int color;

	int stat;
} Item;

typedef struct _Msg Msg;
struct _Msg {
	char *data;
	bool disp;
};

typedef struct _Ent Ent;
struct _Ent {
	char *name;
	ENT_TYPE type;
	ENT_AI ai;
	char face;
	int color;

	DIREC direc;
	int x, y;
	int bary;

	int maxhp, hp;
	bool isdead;
	int damage;
	int sight;
	int speed;

	struct _Msg msg;
	Item inv[MAX_INV];
	Item holding; /* TODO: change to int */
};

/* map.c: handle the map */
void init_map(void);
char get_map(int x, int y);
void set_map(int x, int y, char newch);
bool is_floor(int x, int y);
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
void init_item(void);
void init_entity(void);
void init_player(void);

int maxx;
int maxy;

int itemqty;
int playerqty;
int entqty;

Item item[MAX_ITEMS];
Ent player[MAX_PLAYERS];
Ent entity[MAX_ENTITIES]; /* TODO: rename all ent */

#endif /* RATIUM_H */
