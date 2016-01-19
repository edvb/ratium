/* See LICENSE for licence details. */
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

static void
ratium_help(void) {
	printf("\
Usage: ratium [OPTION]\n\
A dumb little ncurses game where you play as a '@' and have to kill rats\n\
\n\
  -h, --help      display this help and exit\n\
  -v, --version   display version information and exit\n\
\n\
For more info see man page\n\
\n\
ratium home page: <http://edvb.itch.io/ratium>\n\
");
}

int main(int argc, char *argv[]) {
	int optc;
	int c = 0;

	while ((optc = getopt_long(argc, argv, "hv", longopts, NULL)) != -1)
		switch (optc) {
		case 'h':
			ratium_help();
			return 0;
		case 'v':
			printf("ratium v%s\n", VERSION);
			return 0;
		default:
			printf("for help run \"ratium --help\"\n");
			return 1;
		}

	rat_init();

	if (maxx < 80 || maxy < 24) { NERROR("terminal too small"); }

	rat_loop(c);

	rat_cleanup();

	return 0;
}

