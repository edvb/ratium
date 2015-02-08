#include "ratium.h"

int main() {
	initscr();
	/* nodelay(stdscr,true); */
	noecho();
	cbreak();
	curs_set(0);
	nonl();
	keypad(stdscr,true);
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

	char c;

	int maxx;
	int maxy;

	getmaxyx(stdscr, maxy, maxx);

	srand(time(NULL));

	item[0].name = "gold";
	item[0].face = '$';
	item[0].color = YELLOW;
	item[0].type = ITEM_MISC;
	alloc_item(&item[0]);
	item[0].map[7][4]   = item[0].face;
	item[0].map[9][16]  = item[0].face;
	item[0].map[9][17]  = item[0].face;
	item[0].map[11][8]  = item[0].face;
	item[0].map[16][6]  = item[0].face;
	item[0].map[16][13] = item[0].face;
	item[0].map[10][24] = item[0].face;
	item[0].map[14][33] = item[0].face;
	item[0].map[15][33] = item[0].face;
	item[0].map[18][32] = item[0].face;
	item[0].map[21][25] = item[0].face;
	itemCount = 0;

	item[1].name = "sword";
	item[1].face = '/';
	item[1].color = GREY;
	item[1].type = ITEM_SWORD;
	item[1].stat = 3;
	alloc_item(&item[1]);
	item[1].map[4][8]   = item[1].face;
	item[1].map[4][9]   = item[1].face;
	itemCount++;

	item[2].name = "spam";
	item[2].face = '=';
	item[2].color = DARK_YELLOW;
	item[2].type = ITEM_FOOD;
	item[2].stat = 3;
	alloc_item(&item[2]);
	item[2].map[6][4]   = item[2].face;
	item[2].map[5][4]   = item[2].face;
	itemCount++;

	/* door.name = "door"; */
	/* door.face = '+'; */
	/* door.color = BROWN; */
	/* door.type = ITEM_MISC; */
	/* alloc_item(&door); */
	/* door.map[8][8]   = door.face; */
	/* door.map[14][8]  = door.face; */
	/* door.map[7][14]  = door.face; */
	/* door.map[8][19]  = door.face; */
	/* door.map[13][24] = door.face; */
	/* door.map[16][32] = door.face; */
	/* door.map[18][13] = door.face; */
	/* door.map[13][30] = door.face; */
	/* door.map[8][61]  = door.face; */
	/* itemCount++; */

	init_entity(0, 2);
	init_player(0, 0);
	player[0].bary = 0;
	entity[0].bary = 25;
	entity[1].bary = 26;
	entity[2].bary = 27;

	do {

		clear();

		/* TODO: Put these lines into thier own function */
		draw_map();
		for (int i = 0; i <= itemCount; i++)
			draw_item(item[i]);

		player_run(c, &player[0]);
		dumb_ai(&entity[0], player[0].x, player[0].y, 8);
		dumb_ai(&entity[1], player[0].x, player[0].y, 8);
		rand_ai(&entity[2], 8);

		c = getch();

	/* TODO: Find out which loop is the most effective */
	/* } while ("php" != "a good programing language"); */
	/* } while ("apple products" != "good value for their price"); */
	/* } while ("windows" != "a good OS that doesn't scam their users"); */
	} while ("javascript" != "a great and useful tool");
	/* } while ("emacs" != "a tool not bloated in anyway"); */

	endwin();
	printf("GAME OVER\n");

	return 0;
}

