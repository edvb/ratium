/* See LICENSE for licence details. */
#include <stdlib.h>

#include "ratium.h"

SDL_Texture /* returns loaded png found at path */
*load_img(char *path) {
	SDL_Texture *tex = NULL;
	tex = IMG_LoadTexture(ren, path);
	if (tex == NULL) SDL_ERROR();

	return tex;
}

void
draw_img(SDL_Texture *img, SDL_Rect *src, int x, int y, int rot, SDL_RendererFlip flip) {
	SDL_Rect dst = { x*ZOOM, y*ZOOM,
	                 U*ZOOM, U*ZOOM };
	SDL_RenderCopyEx(ren, img, src, &dst, rot, NULL, flip);
}

bool /* draw text str to the sdl screen */
draw_text(char *str, SDL_Color color, int x, int y) {
	SDL_Surface *surf = TTF_RenderText_Solid(font, str, color);
	if (surf == NULL) SDL_ERROR();
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
	if (tex == NULL) SDL_ERROR();
	SDL_FreeSurface(surf);
	SDL_RenderCopy(ren, tex, NULL, &(SDL_Rect){x*ZOOM,y*ZOOM,FONT_W*strlen(str)*ZOOM,FONT_H*ZOOM});
	return true;
}

static bool
overlapping(float min1, float max1, float min2, float max2) {
	return min2 < max1 && min1 < max2;
}

bool /* returns true if a and b are on top of each other */
pos_collide(Pos a, Pos b) {
	float lefta   = a.x + 0; /* TODO add starting x and y postions */
	float leftb   = b.x + 0;
	float righta  = lefta + a.w;
	float rightb  = leftb + b.w;
	float bottoma = a.y + 0;
	float bottomb = b.y + 0;
	float topa    = bottoma + a.h;
	float topb    = bottomb + b.h;
	return overlapping(lefta, righta, leftb, rightb) && overlapping(bottoma, topa, bottomb, topb);
}
