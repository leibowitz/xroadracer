#ifndef __CARLIST_H__
#define __CARLIST_H__

#include "SDL/SDL.h"
#include "struct_car.h"

int init_car_list(SDL_Surface *surface, struct cars *list_cars);

void carlistClear(struct cars **top);

void setNewCarVar(struct cars *car);

struct cars * add_car(struct cars **list_cars);

void clear_cars(
	SDL_Surface *screen, 
	SDL_Surface *background, 
	struct cars *the_player);

#endif
