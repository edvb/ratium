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

	/* TODO: Improve how colors are assigned */
	init_color(0,  100,  100,  100); /* black */
	init_color(1,  600,    0,    0); /* red */
	init_color(2,    0,  600,    0); /* green */
	init_color(3,    0,  400,  800); /* blue */
	init_color(4,  800,  800,    0); /* yellow */
	init_color(5,  370,  280,    0); /* brown */
	init_color(6,  500,  500,  500); /* grey */
	init_color(8,  650,  550,    0); /* dark yellow */
	init_color(9,    0,  400,    0); /* dark green */
	init_color(10,   0,  200,  400); /* dark blue */
	init_color(11, 450,  450,  150); /* haggis */

	init_pair( 1,  1,  0); /* red */
	init_pair( 2,  2,  0); /* green */
	init_pair( 3,  3,  0); /* blue */
	init_pair( 4,  4,  0); /* yellow */
	init_pair( 5,  5,  0); /* brown */
	init_pair( 6,  6,  0); /* grey */
	init_pair( 8,  8,  0); /* dark yellow */
	init_pair( 9,  3, 10); /* water */
	init_pair(10,  2,  0); /* grass */
	init_pair(11, 11,  0); /* haggis */

	char c;

	int maxx;
	int maxy;

	getmaxyx(stdscr, maxy, maxx);

	srand(time(NULL));

	init_entity(0, 2);
	init_player(0, 0);
	player[0].bary = 0;
	entity[0].bary = 25;
	entity[1].bary = 26;
	entity[2].bary = 27;

	alloc_item(0, 3);

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

	for (int i = 0; i <= itemCount; i++)
		free(item[i].name);

	endwin();
	printf("GAME OVER\n");

	return 0;
}

