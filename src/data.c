#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ratium.h"

/* TODO: Improve and implement in other functions */
static void calc_rarity(int *rarity) {
	*rarity = (floor_count('.') % 10) * (*rarity * 1);
	*rarity += rand() % 10;
	if (*rarity + entqty > MAX_ENTITIES)
		*rarity = 0;
}

/* init_item: read from data/items.txt file and store in item array */
void init_item(int from, int to) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char face;
	int color;
	int type;
	int stat;
	int rarity;

	FILE *f = fopen("data/items.txt", "r");

	for (int num = from; num <= to; num++) {
		fscanf(f, "%s %c(%i): type=%i stat=%i rarity=%i\n",
			   name, &face, &color, &type, &stat, &rarity);

		int l = strlen(name);
		for(int i = 0; i < l; i++) {
			if(name[i] == '_') {
				name[i] = ' ';
				continue;
			}
			if(name[i] == ' ')
				break;
		}

		item[num].name = malloc(MAX_NAME * sizeof(char));
		strcpy(item[num].name, name);
		item[num].face = face;
		item[num].color = COLOR_PAIR(color);
		item[num].type = type;
		item[num].stat = stat;

		for (int i = 0; i < MAX_X; i++)
			for (int j = 0; j < MAX_Y; j++)
				item[num].map[j][i] = 0;

		if (rarity != 0)
			for (int x, y, i = 0; i < floor_count('.')/rarity; i++) {
			/* for (int x, y, i = 0; i < calc_rarity(rarity); i++) { */
				do {
					x = rand() % MAX_X;
					y = rand() % MAX_Y;
				} while (get_map(x, y) != '.');
				item[num].map[y][x]++;
			}

	}

	fclose(f);

	free(name);

	itemqty = to;
}

/* init_entity: read from data/entities.txt file and store in entity array */
void init_entity(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char *drop = malloc(MAX_NAME * sizeof(char));
	int type;
	int x_0, y_0;
	char face;
	int color;
	int maxhp;
	int damage;
	int rarity;

	entqty = 0;

	FILE *f = fopen("data/entities.txt", "r");

	do {
		fscanf(f, "%s %c(%i): hp=%i damge=%i type=%i rarity=%i %s\n",
			   name, &face, &color, &maxhp, &damage, &type, &rarity, drop);

		for(int i = 0, l = strlen(name); i < l; i++) {
			if(name[i] == '_') {
				name[i] = ' ';
				continue;
			}
			if(name[i] == ' ')
				break;
		}

		for(int i = 0, l = strlen(drop); i < l; i++) {
			if(drop[i] == '_') {
				drop[i] = ' ';
				continue;
			}
			if(drop[i] == ' ')
				break;
		}

		if (rarity != 0) {
			calc_rarity(&rarity);
			for (int num = entqty; num < rarity; num++, entqty++) {
				entity[num].name = malloc(MAX_NAME * sizeof(char));
				strcpy(entity[num].name, name);
				entity[num].drop = malloc(MAX_NAME * sizeof(char));
				strcpy(entity[num].drop, drop);
				entity[num].face = face;
				entity[num].color = COLOR_PAIR(color);
				entity[num].maxhp = maxhp;
				entity[num].hp = maxhp;
				entity[num].isdead = false;
				entity[num].damage = damage;
				entity[num].type = type;

				entity[num].holding.name = malloc(MAX_NAME * sizeof(char));
				entity[num].holding.face = ' ';
				entity[num].holding.color = 0;
				entity[num].holding.type = 0;
				entity[num].holding.stat = 0;

				for (int i = 0; i < 16; i++) {
					entity[num].inv[i].name = "";
					entity[num].inv[i].face = ' ';
					entity[num].inv[i].color = 0;
					entity[num].inv[i].qty = 0;
				}

				/* TODO: Break into function and add smart
				 * intergration of is_floor function */
				do {
					x_0 = rand() % MAX_X;
					y_0 = rand() % MAX_Y;
				} while (get_map(x_0, y_0) != '.');
				entity[num].x = x_0;
				entity[num].y = y_0;

			}
		}
	} while (!feof(f));

	fclose(f);

	free(name);
	free(drop);

}

/* init_npc: read from data/npcs.txt file and store in npc array */
void init_npc(void) {

	char *name = malloc(MAX_NAME * sizeof(char));
	char *drop = malloc(MAX_NAME * sizeof(char));
	char *message = malloc(MAX_NAME * sizeof(char));
	int type;
	int x_0, y_0;
	char face;
	int color;
	int maxhp;
	int damage;
	int rarity;

	npcqty = 0;

	FILE *f = fopen("data/npcs.txt", "r");

	do {
		fscanf(f, "%s %c(%i): hp=%i damge=%i type=%i rarity=%i %s %s\n",
			   name, &face, &color, &maxhp, &damage, &type, &rarity, drop, message);

		for(int i = 0, l = strlen(name); i < l; i++) {
			if(name[i] == '_') {
				name[i] = ' ';
				continue;
			}
			if(name[i] == ' ')
				break;
		}

		for(int i = 0, l = strlen(drop); i < l; i++) {
			if(drop[i] == '_') {
				drop[i] = ' ';
				continue;
			}
			if(drop[i] == ' ')
				break;
		}

		for(int i = 0, l = strlen(message); i < l; i++) {
			if(message[i] == '_') {
				message[i] = ' ';
				continue;
			}
			if(message[i] == ' ')
				break;
		}

		if (rarity != 0) {
			calc_rarity(&rarity);
			for (int num = npcqty; num < 1; num++, npcqty++) {
				npc[num].e.name = malloc(MAX_NAME * sizeof(char));
				strcpy(npc[num].e.name, name);
				npc[num].e.drop = malloc(MAX_NAME * sizeof(char));
				strcpy(npc[num].e.drop, drop);
				npc[num].message = malloc(MAX_NAME * sizeof(char));
				strcpy(npc[num].message, message);
				npc[num].e.face = face;
				npc[num].e.color = COLOR_PAIR(color);
				npc[num].e.maxhp = maxhp;
				npc[num].e.hp = maxhp;
				npc[num].e.isdead = false;
				npc[num].e.damage = damage;
				npc[num].e.type = type;

				npc[num].e.holding.name = malloc(MAX_NAME * sizeof(char));
				npc[num].e.holding.face = ' ';
				npc[num].e.holding.color = 0;
				npc[num].e.holding.type = 0;
				npc[num].e.holding.stat = 0;

				for (int i = 0; i < 16; i++) {
					npc[num].e.inv[i].name = "";
					npc[num].e.inv[i].face = ' ';
					npc[num].e.inv[i].color = 0;
					npc[num].e.inv[i].qty = 0;
				}

				/* TODO: Break into function and add smart
				 * intergration of is_floor function */
				do {
					x_0 = rand() % MAX_X;
					y_0 = rand() % MAX_Y;
				} while (get_map(x_0, y_0) != '.');
				npc[num].e.x = x_0;
				npc[num].e.y = y_0;

			}
		}
	} while (!feof(f));

	fclose(f);

	free(name);
	free(drop);
	free(message);

}

/* init_player: read from data/players.txt file and store in player array */
void init_player(int from, int to) {

	char *name = malloc(MAX_NAME * sizeof(char));
	int x_0, y_0;
	char face;
	int color;
	int maxhp;
	int damage;

	FILE *f = fopen("data/players.txt", "r");

	for (int num = from; num <= to; num++) {
		fscanf(f, "%s %c(%i): hp=%i damge=%i\n",
			   name, &face, &color, &maxhp, &damage);

		int l = strlen(name);
		for(int i = 0; i < l; i++) {
			if(name[i] == '_') {
				name[i] = ' ';
				continue;
			}
			if(name[i] == ' ')
				break;
		}

		player[num].name = malloc(MAX_NAME * sizeof(char));
		strcpy(player[num].name, name);
		player[num].face = face;
		player[num].color = COLOR_PAIR(color);
		player[num].maxhp = maxhp;
		player[num].hp = maxhp;
		player[num].damage = damage;

		player[num].holding.name = malloc(MAX_NAME * sizeof(char));
		player[num].holding.face = ' ';
		player[num].holding.color = 0;
		player[num].holding.type = 0;
		player[num].holding.stat = 0;

		do {
			x_0 = rand() % MAX_X;
			y_0 = rand() % MAX_Y;
		} while (!is_floor(x_0, y_0));
		player[num].x = x_0;
		player[num].y = y_0;

		for (int i = 0; i < 16; i++) {
			player[num].inv[i].name = "";
			player[num].inv[i].face = ' ';
			player[num].inv[i].color = 0;
			player[num].inv[i].qty = 0;
		}
	}

	fclose(f);

	free(name);

	playerqty = to;
}

