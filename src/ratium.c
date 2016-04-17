/* See LICENSE for licence details. */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ratium.h"

static const struct option longopts[] = {
	{"help",    no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'},
	{NULL,      0,           NULL, 0  }
};

static void
ratium_help(void) {
	printf("\
Usage: ratium [OPTION]\n\
A dumb little ncurses game where you play as a '@' and have to kill rats\n\
\n\
      --help      display this help and exit\n\
      --version   output version information and exit\n\
\n\
For more info see man page\n\
\n\
ratium home page: <http://edvb.itch.io/ratium>\n\
");
}

int main(int argc, char *argv[]) {
	int optc;

	while ((optc = getopt_long(argc, argv, "", longopts, NULL)) != -1)
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

	if (!rat_init()) return 1;

	rat_loop();

	rat_cleanup();

	return 0;
}

