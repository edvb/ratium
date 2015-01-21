#include "ratium.h"

void toggle_door(int x, int y) {
	if (door.map[y-1][x] == '+')
		door.map[y-1][x] = '-';
	else if (door.map[y+1][x] == '+')
		door.map[y+1][x] = '-';
	else if (door.map[y][x-1] == '+')
		door.map[y][x-1] = '-';
	else if (door.map[y][x+1] == '+')
		door.map[y][x+1] = '-';
	else if (door.map[y-1][x] == '-')
		door.map[y-1][x] = '+';
	else if (door.map[y+1][x] == '-')
		door.map[y+1][x] = '+';
	else if (door.map[y][x-1] == '-')
		door.map[y][x-1] = '+';
	else if (door.map[y][x+1] == '-')
		door.map[y][x+1] = '+';
}
