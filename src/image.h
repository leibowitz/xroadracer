#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <SDL/SDL.h>
#include "struct_car.h"
#include "map_struct.h"

SDL_Surface * Load_img_from(char *file);

void show_img_xy(
	SDL_Surface *screen, 
	char img[], 
	int x, 
	int y);

SDL_Surface * show_splash(
	SDL_Surface *screen, 
	char img[]);

SDL_Surface* surface_from_img(
	char *img_name);


void show_start_light_img_on_the_middle(
	SDL_Surface *screen,
	SDL_Surface *background,
	int type);

void copySurfaceToScreen(
	SDL_Surface *screen,
	SDL_Surface *img,
	int x, int y, int w, int h);

void copy_car_toscreen(
	SDL_Surface * pcar_buffer,
	SDL_Surface *pscreen, 
	SDL_Rect *pdest,
	struct cars *p);

void showStartLight(SDL_Surface *screen,
		int whatLight, 
		SDL_Surface **s);

void blitStartLight(struct mapData *mapInfos, 
		SDL_Surface *s);

#endif
