/* See LICENSE for licence details. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ratium.h"

#define SDL_ERROR() { \
	printf("SDL Error: %s\n", SDL_GetError()); \
	return false; \
}

bool
rat_init(void) {
	ZOOM = 2;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) SDL_ERROR()

	win = NULL;
	win = SDL_CreateWindow("ratium", 100, 100, MAX_X*ZOOM*U, MAX_Y*ZOOM*U, SDL_WINDOW_SHOWN);
	ren = NULL;
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (win == NULL || ren == NULL) SDL_ERROR()

	if (TTF_Init() != 0) SDL_ERROR()
	if ((font = TTF_OpenFont("data/Ricasso.ttf", 80)) == NULL) SDL_ERROR()

	SDL_SetRenderDrawColor(ren, 0x20, 0x20, 0x20, 0x20);
	IMG_Init(IMG_INIT_PNG);

	nullimg = load_img("data/null.png");

	srand(time(NULL));

	init_map();
	init_block();
	init_entity();
	init_player(DEF_PLAYERS);
	init_item();

	return true;
}

void rat_loop(void) {
	SDL_Event e;
	Uint32 ticksstep = 2000 / 30;
	Uint32 ticksnow = SDL_GetTicks();
	Uint32 tickselapsed = 0;

	do {
		const Uint32 tickslast = ticksnow;
		ticksnow = SDL_GetTicks();
		tickselapsed += ticksnow - tickslast;
		if ((int)tickselapsed < 1000 / 30) {
			SDL_Delay(1);
			continue;
		}

		SDL_RenderClear(ren);

		for (int i = 0; i < entqty; i++)
			if (SDL_GetTicks() % 10 == 0) /* TODO: improve */
				entity[i].run(&entity[i]);

		for (int i = playerqty; i >= 0; i--) {
			draw_map(player[i], player[i].sight);
			for (int j = itemqty; j >= 0; j--)
				draw_item(item[j], player[i], player[i].sight);
		}
		for (int i = playerqty; i >= 0; i--) {
			draw_ent(player[i], player[i], player[i].sight);
			for (int j = entqty; j >= 0; j--)
				draw_ent(entity[j], player[i], player[i].sight);
		}

		for (int i = 0; i <= playerqty; i++)
			draw_msg(player[i].msg);

		SDL_RenderPresent(ren);

		for (int i = 0; i <= playerqty; i++)
			while (tickselapsed >= ticksstep) {
				player_run(&player[i]);
				tickselapsed -= ticksstep;
			}

		SDL_PollEvent(&e);
	} while (e.key.keysym.sym != SDLK_ESCAPE);
}

void rat_cleanup(void) {
	SDL_DestroyWindow(win);
	win = NULL;
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	printf("GAME OVER\n");

	for (int i = 0; i <= blockqty; i++) {
		free(block[i].name);
		SDL_DestroyTexture(block[i].img);
	}
	for (int i = 0; i <= itemqty; i++) {
		free(item[i].name);
		SDL_DestroyTexture(item[i].img);
	}
	for (int i = 0; i <= entqty; i++) {
		free(entity[i].name);
		free(entity[i].msg);
		for (int j = 0; j < MAX_INV; j++)
			free(entity[i].inv[j].name);
		SDL_DestroyTexture(entity[i].img);
	}
	for (int i = 0; i <= playerqty; i++) {
		free(player[i].name);
		player[i].msg = NULL;
		player[i].msg = NULL;
		free(player[i].msg);
		for (int j = 0; j < MAX_INV; j++)
			free(player[i].inv[j].name);
		SDL_DestroyTexture(player[i].img);
	}

}
