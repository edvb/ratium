typedef struct _Map Map;
struct _Map {
	int len, height;
	char map[MAX_Y][MAX_X+1]; /* +1 for \0 */
};

char worldMap[MAX_Y][MAX_X+1] = {
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggg###################ggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggg#.................######gggggggggggggggggggggggggggggg",
"ggg#####++#####ggggggggggg#.................+....#gggggggggggggggggggggggggggggg",
"ggg#..........#ggggggggggg##############+####....#gggggggggggggggggggggggggggggg",
"ggg#..........######ggggggggggggggggggg#.#gg######gggggggggggggggggggggggggggggg",
"ggg#..........#....#ggggggggggggggggggg#.#gggggggggggggg..gggggggggggggggggggggg",
"ggg#..........+....#######ggg###########.################..ggggggggggggggggggggg",
"ggg#####+######..........#ggg#..........................+..ggggggggggggggggggggg",
"ggggggg#.#gggg#....#####.#ggg#.#################+########.g.gggggggggggggggggggg",
"ggggggg#.#gggg######ggg#.#ggg#.#gggggggggggggg#...#gggg...gggggggggggggggggggggg",
"ggggggg+.+ggggggggggggg#.#ggg#.#gggggggggggggg#...#ggggg.ggggggggggggggggggggggg",
"ggggggg#.#ggggggggggggg#.#ggg#.#gggggggggggggg#####ggggggggggggggggggggggggggggg",
"ggggggg#.#ggggggggggg###+#####+####ggggggggggggggggggggggggggggggggggggggggggggg",
"gggg####.#######ggggg#............#gggggggggggggggggggggggggggggggggggggggggggg#",
"gggg#..........#ggggg#............#gggggggggggggggggggggggggggggggggggggggg#####",
"gggg#..........#ggggg###########+##ggggggggggggggggggggggggggggggggggggg########",
"gggg+..........#ggggggggggggggg#.#ggggggggggggggggggggggggggggggggggg###########",
"gggg#########+##ggggggggggggggg+.#ggggggggggggggggggggggggggggggggg#############",
"gggggggggggg#.#gggggggggggggggg#.#gggggggggggggggggggggggggggggggg##############",
"gggggggggggg#.##################.#ggggggggggggggggggggggggggggggg###############",
"gggggggggggg#....................#ggggggggggggggggggggggggggggggg###############",
"gggggggggggg######################gggggggggggggggggggggggggggggg################",
"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg################"
};

int maprand[MAX_Y][MAX_X+1];

struct _Map buildings[10] = {
{ 12, 5, {
"XXXXXXXX+XXX",
"X..........X",
"X..........X",
"X..........X",
"XXXXXXXXXXXX" } },
{  8, 6, {
" wwwww  ",
"wwwwwww ",
"wwwwwwww",
"  wwwwww",
"   wwww ",
"    ww  " } },
{  3, 3, {
" # ",
"#w#",
" # " } }
};

