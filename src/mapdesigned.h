#ifndef __MAPDESIGNED_H__
#define __MAPDESIGNED_H__

#include "SDL/SDL.h"

SDL_Surface* load_map_image(
	char *map_name,
	char *format);

SDL_Surface* load_background_map(
	SDL_Surface *screen, 
	char *map_name);
	
SDL_Surface* load_checkpoints(
	SDL_Surface *screen, 
	char *map_name);
	
#endif
