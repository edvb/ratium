typedef struct _Map Map;
struct _Map {
	size_t len, height;
	int rarity;
	char map[MAX_Y][MAX_X+1]; /* +1 for \0 */
};

char worldMap[MAX_Y][MAX_X+1] = {
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"ggg#####++#####ggggggggggggggggggggggggggggggggg",
"ggg#..........#ggggggggggggggggggggggggggggggggg",
"ggg#..........######gggggggggggggggggggggggggggg",
"ggg#..........#....#gggggggggggggggggggggggggggg",
"ggg#..........+....#######gggggggggggggggggggggg",
"ggg#####+######..........#gggggggggggggggggggggg",
"ggggggg#.#gggg#....#####.#gggggggggggggggggggggg",
"ggggggg#.#gggg######ggg#.#gggggggggggggggggggggg",
"ggggggg+.+ggggggggggggg#.#gggggggggggggggggggggg",
"ggggggg#.#ggggggggggggg#.#gggggggggggggggggggggg",
"ggggggg#.#ggggggggggg###+#####+####ggggggggggggg",
"gggg####.#######ggggg#............#ggggggggggggg",
"gggg#..........#ggggg#............#ggggggggggggg",
"gggg#..........#ggggg###########+##ggggggggggggg",
"gggg+..........#ggggggggggggggg#.#gggggggggggggg",
"gggg#########+##ggggggggggggggg+.#gggggggggggggg",
"gggggggggggg#.#gggggggggggggggg#.#gggggggggggggg",
"gggggggggggg#.##################.#gggggggggggggg",
"gggggggggggg#....................#gggggggggggggg",
"gggggggggggg######################gggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
"gggggggggggggggggggggggggggggggggggggggggggggggg",
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

