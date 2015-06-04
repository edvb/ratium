#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ratium.h"
#include "gfx.h"

/* display error message and quit */
#define ERROR(msg) printf("%s: error: %s\n", argv[0], msg); \
		   return 1;
/* end ncurses then run ERROR */
#define NERROR(msg) rat_endwin(); ERROR(msg);

static const struct option longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};

static void ratium_help(void) {
	printf("\
Usage: ratium [OPTION]\n\
A dumb little ncurses game where you play as a '@' and have to kill rats\n\
\n\
  -h, --help      display this help and exit\n\
  -v, --version   display version information and exit\n\
\n\
For more info see man page\n\
\n\
ratium home page: <https://github.com/edvb54/ratium-c>\n\
");
}

static void ratium_version(void) {
	printf("ratium v%s\n", VERSION);
}

int main(int argc, char *argv[]) {
	int optc;

	while ((optc = getopt_long(argc, argv, "hv", longopts, NULL)) != -1)
		switch (optc) {
		case 'h':
			ratium_help();
			return 0;
		case 'v':
			ratium_version();
			return 0;
		default:
			printf("for help run \"ratium --help\"\n");
			return 1;
		}

	rat_init();
	rat_start_color();

	int c;

	rat_getmaxxy();

	if (maxx < 80 || maxy < 24) { NERROR("terminal too small"); }

	srand(time(NULL));

	init_map();
	if (!init_entity())  { NERROR("file data/entities.txt not found"); }
	if (!init_player(2)) { NERROR("file data/players.txt not found"); } /* TODO: Add option to change this */
	if (!init_item())    { NERROR("file data/items.txt not found"); }

	do {
		rat_clear();

		for (int i = 0; i < entqty; i++)
			switch (entity[i].ai) {
			case AI_PLAYER: break;
			case AI_HOSTILE:
				dumb_ai(&entity[i], player[0].x, player[0].y, entity[i].speed);
				break;
			case AI_PEACEFUL:
				rand_ai(&entity[i], entity[i].speed);
				break;
			}

		for (int i = playerqty; i >= 0; i--) {
			draw_map_floor(player[i], player[i].sight);
			for (int j = itemqty; j >= 0; j--)
				draw_item(item[j], player[i], player[i].sight);
		}
		for (int i = playerqty; i >= 0; i--) {
			draw_ent(player[i], player[i], player[i].sight);
			for (int j = entqty; j >= 0; j--)
				draw_ent(entity[j], player[i], player[i].sight);
			draw_map(player[i], player[i].sight);
		}

		for (int i = 0; i <= playerqty; i++) /* TODO: Move */
			draw_msg(&player[i].msg);

		c = rat_getch();

		for (int i = 0; i <= playerqty; i++)
			while (!player_run(c, &player[i]) && c != RAT_ESC) /* TODO: 27 > ESC */
				c = rat_getch();

	} while (c != RAT_ESC);

	rat_endwin();
	printf("GAME OVER\n");

	for (int i = 0; i <= playerqty; i++)
		for (int j = 0; i < MAX_INV; i++)
			if (player[i].inv[j].face == '$')
				printf("%s's Score: %d\n",
				        player[i].name, player[i].inv[j].map[0][0]);

	for (int i = 0; i <= itemqty; i++)
		free(item[i].name);
	for (int i = 0; i <= entqty; i++) {
		free(entity[i].name);
		free(entity[i].holding.name);
		free(entity[i].msg.data);
		for (int j = 0; j < MAX_INV; j++)
			free(entity[i].inv[j].name);
	}
	for (int i = 0; i <= playerqty; i++) {
		free(player[i].name);
		free(player[i].msg.data);
		for (int j = 0; j < MAX_INV; j++)
			free(player[i].inv[j].name);
		free(player[i].holding.name);
	}

	return 0;
}

