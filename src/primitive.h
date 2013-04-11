#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "SDL/SDL.h"

void drawPointPattern(
	int x, 
	int y, 
	Uint32 color, 
	SDL_Surface *sDest);

void drawLineAB(
	float x1, 
	float y1, 
	float x2, 
	float y2, 
	Uint32 color, 
	SDL_Surface *sDest);
	
#endif
