#ifndef __CUSTOM_H__
#define __CUSTOM_H__

#include "SDL/SDL.h"

SDL_Surface* customLoadSurface(
	SDL_Surface *screen, 
	int width,
	int height,
	Uint32 color);

void customLoadCheckp(
	SDL_Surface *checkpoints, 
	char *map_name);
	
void customLoadPieces(
	SDL_Surface *background, 
	char *map_name);
	


#endif
