/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"

SDL_Texture /* returns loaded png found at path */
*load_img(char *path) {
	SDL_Texture *tex = NULL;
	tex = IMG_LoadTexture(ren, path);
	if (tex == NULL) {
		printf("SDL_image Error: %s\n", path);
		return nullimg;
	}

	return tex;
}

void
draw_img(SDL_Texture *img, SDL_Rect *src, int x, int y, SDL_RendererFlip flip) {
	SDL_Rect dst = { x*ZOOM, y*ZOOM,
	                 U*ZOOM, U*ZOOM };
	SDL_RenderCopyEx(ren, img, src, &dst, 0, NULL, flip);
}

void /* draw text str to the sdl screen */
draw_text(char *str, SDL_Color color, int x, int y) {
	SDL_Surface *surf = TTF_RenderText_Solid(font, str, color);
	if (surf == NULL) {
		printf("SDL Error: %s\n", SDL_GetError());
		return;
	}
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
	if (tex == NULL) {
		printf("SDL Error: %s\n", SDL_GetError());
		return;
	}
	SDL_FreeSurface(surf);
	SDL_RenderCopy(ren, tex, NULL, &(SDL_Rect){x*ZOOM,y*ZOOM,20*strlen(str)*ZOOM,40*ZOOM});
	return;
}

