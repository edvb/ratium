#include "ratium.h"

int main() {
	initscr();
	/* nodelay(stdscr,true); */
	noecho();
	cbreak();
	curs_set(0);
	nonl();
	keypad(stdscr,true);
	/* timeout(0); */
	scrollok(stdscr, false);

	start_color();

	init_color(0,  100,  100,  100); /* black */
	init_color(1,  600,    0,    0); /* red */
	init_color(2,    0,  600,    0); /* green */
	init_color(3,    0,  400,  800); /* blue */
	init_color(4,  800,  800,    0); /* yello */
	init_color(5,  370,  280,    0); /* brown */
	init_color(6,  500,  500,  500); /* grey */
	init_color(8,  650,  550,    0); /* dark yellow */
	init_color(9,    0,  400,    0); /* dark green */
	init_color(10,   0,  200,  400); /* dark blue */

	init_pair( 1,  1,  0); /* red */
	init_pair( 2,  2,  0); /* green */
	init_pair( 3,  3,  0); /* blue */
	init_pair( 4,  4,  0); /* yellow */
	init_pair( 5,  5,  0); /* brown */
	init_pair( 6,  6,  0); /* grey */
	init_pair( 8,  8,  0); /* dark yellow */
	init_pair( 9,  3, 10); /* water */
	init_pair(10,  2,  0); /* grass */

	bool on = true;

	char c;

	int maxx;
	int maxy;

	getmaxyx(stdscr, maxy, maxx);

	gold.face = '$';
	gold.color = COLOR_PAIR(4);
	gold.map[7][4]   = gold.face;
	gold.map[9][16]  = gold.face;
	gold.map[9][17]  = gold.face;
	gold.map[11][8]  = gold.face;
	gold.map[16][6]  = gold.face;
	gold.map[16][13] = gold.face;
	gold.map[10][24] = gold.face;
	gold.map[14][33] = gold.face;
	gold.map[15][33] = gold.face;
	gold.map[18][32] = gold.face;
	gold.map[21][25] = gold.face;
	init_item(gold);

	entity[0].x = 5;
	entity[0].y = 5;
	entity[0].face = '@';
	entity[0].color = COLOR_PAIR(3);

	entity[1].x = 12;
	entity[1].y = 5;
	entity[1].face = 'r';
	entity[1].color = COLOR_PAIR(5);

	do {

		clear();

		draw_map();
		draw_item(gold);

		player_run(c, &entity[0]);

		for (int i = 0; i <= 1; i++)
			mvaddch(entity[i].y, entity[i].x, entity[i].face + entity[i].color);

		c = getch();

	} while (on);

	endwin();
	printf("GAME OVER\n");

	return 0;
}

