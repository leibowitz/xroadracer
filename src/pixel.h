#ifndef __PIXEL_H__
#define __PIXEL_H__

#include "SDL/SDL.h"

int check_pixel_xy(
	SDL_Surface * screen, 
	double dot_x, double dot_y,
	SDL_Surface *background,
	int xvel,
	SDL_Rect map_offset);

Uint32 getpixel(
	SDL_Surface *surface, 
	int x, 
	int y);
	
void putpixel(
	SDL_Surface 
	*surface, 
	int x, 
	int y, 
	Uint32 pixel);

#endif
