#ifndef GFX_H
#define GFX_H

#ifdef RAT_NCURSES
#  define RAT_ESC 27 /* ncurses escape code */
#  define RAT_REVERSE ((1Ul) << 18)
#endif

void rat_init();
void rat_start_color();
void rat_getmaxxy();
void rat_clear();
int  rat_getch();
void rat_mvaddch(int x, int y, char face, int color);
void rat_mvprint(int x, int y, char *data, int color);
void rat_print(char *data, int color);
void rat_endwin();

#endif /* GFX_H */