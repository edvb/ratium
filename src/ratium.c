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

	srand(time(NULL));

	gold.name = "gold";
	gold.face = '$';
	gold.color = YELLOW;
	alloc_item(&gold);
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
	itemCount = 0;

	item[2].name = "spam";
	item[2].face = '=';
	item[2].color = DARK_YELLOW;
	alloc_item(&item[2]);
	item[2].map[6][4]   = item[2].face;
	item[2].map[5][4]   = item[2].face;
	itemCount++;

	door.name = "door";
	door.face = '+';
	door.color = BROWN;
	alloc_item(&door);
	door.map[8][8]   = door.face;
	door.map[14][8]  = door.face;
	door.map[7][14]  = door.face;
	door.map[8][19]  = door.face;
	door.map[13][24] = door.face;
	door.map[16][32] = door.face;
	door.map[18][13] = door.face;
	door.map[13][30] = door.face;
	door.map[8][61]  = door.face;
	itemCount++;

	init_entity(0, 3);
	entity[0].bary = 0;
	entity[1].bary = 25;
	entity[2].bary = 26;
	entity[3].bary = 27;

	do {

		clear();

		draw_map();

		for (int i = 0; i <= itemCount; i++)
			draw_item(item[i]);

		player_run(c, &entity[0]);
		dumb_ai(&entity[1], entity[0].x, entity[0].y, 8);
		dumb_ai(&entity[2], entity[0].x, entity[0].y, 8);
		rand_ai(&entity[3], 8);

		c = getch();

	} while (on);

	endwin();
	printf("GAME OVER\n");

	return 0;
}

