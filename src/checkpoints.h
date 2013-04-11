#ifndef __CHECKPOINTS_H__
#define __CHECKPOINTS_H__

#include "SDL/SDL.h"
#include "struct_car.h"

void next_checkpoint(struct cars *car_player, int color);

void missed_checkpoint_return(struct cars *car_player, SDL_Surface *screen);

#endif 
