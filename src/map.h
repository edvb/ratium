typedef struct _Map Map;
struct _Map {
	size_t len, height;
	int rarity;
	char map[MAX_Y][MAX_X+1]; /* +1 for \0 */
};

char worldMap[MAX_Y][MAX_X+1];

struct _Map buildings[10] = {
{ 8, 6, 2, {
" wwwww  ",
"wwwwwww ",
"wwwwwwww",
"  wwwwww",
"   wwww ",
"    ww  " }},
{ 8, 6, 2, {
"   www  ",
"  wwwww ",
" wwwwww ",
"wwwwww  ",
"wwwwww  ",
" ww     " }},
{ 9, 9, 1, {
"XXWXXWWXX",
"XhohbbbbX",
"XbbbboooX",
"XhohbbbbX",
"Xbbbbbb0X",
"Xhohbbb0X",
"XXWWX+XXX",
" h hddddd",
"     dd  ",
}},
{ 10, 10, 1, {
"XXWXWXWXXX",
"XbbbbbbbbX",
"XooooooobX",
"XhhhhhhhbX",
"XhbbbbbbbX",
"XobbhohbhX",
"XhbbbbbhoX",
"XXW+XWWXXX",
" ddddd h h",
"   dd     ",
}},
};

