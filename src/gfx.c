/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"

#ifdef RAT_NCURSES
#  include <ncurses.h>
#endif

#include "gfx.h"

void rat_init_gfx() {
#ifdef RAT_NCURSES
	initscr();
	/* nodelay(stdscr,true); */
	noecho();
	cbreak();
	curs_set(0);
	nonl();
	keypad(stdscr,true);
	scrollok(stdscr, false);
#endif
}

void rat_start_color() {
#ifdef RAT_NCURSES
	/* TODO: Improve how colors are assigned */
	if (start_color() == 0) {
		init_color( 0, 100,  100,  100); /* black */
		init_color( 1, 600,    0,    0); /* red */
		init_color( 2,   0,  600,    0); /* green */
		init_color( 3,   0,  400,  800); /* blue */
		init_color( 4, 800,  800,    0); /* yellow */
		init_color( 5, 370,  280,    0); /* brown */
		init_color( 6, 500,  500,  500); /* grey */
		init_color( 8, 650,  550,    0); /* dark yellow */
		init_color( 9,   0,  400,    0); /* dark green */
		init_color(10,   0,  200,  400); /* dark blue */
		init_color(11, 200,  200,  200); /* dark grey */
		init_color(12, 150,  120,   50); /* dark brown */

		init_pair( 1,  1,  0); /* red */
		init_pair( 2,  2,  0); /* green */
		init_pair( 3,  3,  0); /* blue */
		init_pair( 4,  4,  0); /* yellow */
		init_pair( 5,  5,  0); /* brown */
		init_pair( 6,  6,  0); /* grey */
		init_pair( 8,  8,  0); /* dark yellow */
		init_pair( 9,  3,  0); /* water */
		init_pair(10,  2,  0); /* grass */
		init_pair(11, 11,  0); /* dark grey */
		init_pair(12,  6, 11); /* dark grey bg */
		init_pair(13,  5, 12); /* dark brown bg */
	} else {
		endwin();
		printf("ratium: error: terminal does not support colors\n");
		exit(1);
	}
#endif
}

void rat_getmaxxy() {
#ifdef RAT_NCURSES
	getmaxyx(stdscr, maxy, maxx);
#endif
}

void rat_clear() {
#ifdef RAT_NCURSES
	clear();
#endif
}

int rat_getch() {
#ifdef RAT_NCURSES
	return getch();
#endif
}

void rat_mvaddch(int x, int y, char face, int color) {
#ifdef RAT_NCURSES
	attron(COLOR_PAIR(color));
	mvaddch(y, x, face);
	attroff(COLOR_PAIR(color));
#endif
}

void rat_mvprint(int x, int y, char *data, int color) {
#ifdef RAT_NCURSES
	if (color == -1)
		attron(RAT_REVERSE);
	else
		attron(COLOR_PAIR(color));
	mvprintw(y, x, data);
	attroff(COLOR_PAIR(color));
	attroff(RAT_REVERSE);
#endif
}

void rat_print(char *data, int color) {
#ifdef RAT_NCURSES
	if (color == -1) /* TODO: Improve so other types can be used */
		attron(RAT_REVERSE);
	else
		attron(COLOR_PAIR(color));
	printw(data);
	attroff(COLOR_PAIR(color));
	attroff(RAT_REVERSE);
#endif
}

void rat_endwin() {
#ifdef RAT_NCURSES
	endwin();
#endif
}

