#ifndef __INIT_H__
#define __INIT_H__

#include "SDL/SDL.h"
#include "map_struct.h"

SDL_Surface* initScreen();

void initCustomMapData(
	SDL_Surface *screen,
	char *str,
	struct mapData *mapInfos);
	/*SDL_Surface **bg,
	SDL_Surface **cp);*/
	
int initFont();

#endif
