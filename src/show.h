#ifndef __SHOW_H__
#define __SHOW_H__

#include "map_struct.h"

void rotateCar(struct mapData *mapInfos,
	      struct cars *car_player);

void showCarOnScreen(struct mapData *mapInfos,
		struct cars *p);

#endif
