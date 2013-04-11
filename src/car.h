#ifndef __CAR_H__
#define __CAR_H__

#include "struct_car.h"
#include "SDL/SDL.h"

void carReactionMove(
	struct cars *one_car, 
	double ang, 
	double dif, 
	int coef,
	int frameTime);

void carReaction(
	struct cars *one_car, 
	double cos, 
	double sin, 
	SDL_Surface *background,
	int frameTime);

void carOnEdge(SDL_Surface *background, struct cars *one_car, int frameTime);


void carCollision (struct cars *car1, struct cars *car2, SDL_Surface *background, int millitime);


#endif
