typedef struct _Map Map;
struct _Map {
	size_t len, height;
	int rarity;
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

struct _Map buildings[10] = {
{ 8, 6, 1, {
" wwwww  ",
"wwwwwww ",
"wwwwwwww",
"  wwwwww",
"   wwww ",
"    ww  " }},
{ 3, 3, 1, {
" # ",
"#w#",
" # " }},
{ 4, 4, 3, {
" ## ",
"####",
" ###",
"  # " }}
};

