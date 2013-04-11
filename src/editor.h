#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define MAX_MAP_WIDTH 250
#define MAX_MAP_HEIGHT 250
#define CONSTRUCT_MAX_PIECES 6

/* --------------------- */
struct coordPt {
	int x;
	int y;
};
/* --------------------- */
struct constructPieces {
	int x;
	int y;
	int width;
	int height;
	SDL_Surface *img;
	Uint32 colorKey;
	int pieceNumber;
	int angRotation;
	struct constructPieces *next;
};
/* --------------------- */

void startNewEditor(
	SDL_Surface *screen,
	TTF_Font *font,
	SDL_Joystick *joy);

#endif
