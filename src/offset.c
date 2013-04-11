#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

#include "map_struct.h"
#include "struct_car.h"
#include "main.h"

void offsetSub(double carPosition, 
		Sint16 *offset, 
		int screenLength,
		int maxOffset)
{
	if(carPosition - *offset != screenLength)
	{
		*offset = carPosition - screenLength;

		if(*offset > maxOffset)
			*offset = maxOffset;

		if(*offset < 0)
			*offset = 0;
	}
}

void updateMapOffset(struct mapData *mapInfos)
{
	struct cars *car_player;

	car_player = mapInfos->p;
	
	while(car_player != NULL)
	{
		if(car_player -> my_car.computer == 0)
		{
			offsetSub(car_player->my_car.car_x,
				&(car_player->my_car.offsetView.x),
				(car_player->my_car.destView.w/2),
				car_player->my_car.maxWidthView);
			
			offsetSub(car_player->my_car.car_y,
				&(car_player->my_car.offsetView.y),
				(car_player->my_car.destView.h/2),
				car_player->my_car.maxHeightView);

			/*printf("%s %d %d\n",
				car_player->my_car.player_name,
				car_player->my_car.offsetView.x,
				car_player->my_car.offsetView.y);*/

			/* old code */
			/*
			offsetSub(car_player->my_car.car_x,
					&(mapInfos->mapOffset.x),
					(mapInfos->screen->w/2),
					mapInfos->maxWidth);


			offsetSub(car_player->my_car.car_y,
					&(mapInfos->mapOffset.y),
					(mapInfos->screen->h/2),
					mapInfos->maxHeight);
			*/
			/*if(car_player->my_car.car_x - 
				mapInfos->mapOffset.x !=  
				(mapInfos->screen->w/2))
			{
				
				mapInfos->mapOffset.x = car_player->my_car.car_x-
					mapInfos->screen->w/2;
				
				if(mapInfos->mapOffset.x > 
						mapInfos->maxWidth)
					mapInfos->mapOffset.x = 
						mapInfos->maxWidth;

				if(mapInfos->mapOffset.x < 0)
					mapInfos->mapOffset.x = 0;
			}*/
			/*
			if(car_player->my_car.car_x - 
				mapInfos->mapOffset.x < 
				(mapInfos->screen->w/2) && 
				mapInfos->mapOffset.x > 0)
			{
					mapInfos->mapOffset.x;
				if(mapInfos->mapOffset.x < 0)
					mapInfos->mapOffset.x = 0;
			}
				
				if(
					mapInfos->mapOffset.x < 
						mapInfos->maxWidth && 
					car_player->my_car.last_x < 
					car_player->my_car.car_x)
				{
					mapInfos->mapOffset.x +=
						nearbyint(-car_player->
							my_car.last_x+
						car_player->my_car.car_x);

					if(mapInfos->mapOffset.x > mapInfos->maxWidth)
						mapInfos->mapOffset.x = mapInfos->maxWidth;

					if(mapInfos->mapOffset.x < 0)
						mapInfos->mapOffset.x = 0;
				
				}

			if(car_player->my_car.last_x > 
					car_player->my_car.car_x)
			{
				mapInfos->mapOffset.x -=
					nearbyint(car_player->
							my_car.last_x -
					car_player->my_car.car_x);
				
				if(mapInfos->mapOffset.x > mapInfos->maxWidth)
					mapInfos->mapOffset.x = mapInfos->maxWidth;

				if(mapInfos->mapOffset.x < 0)
					mapInfos->mapOffset.x = 0;
			}

			if(car_player->my_car.car_y - mapInfos->mapOffset.y 
				< (SCREEN_HEIGHT/2) && mapInfos->mapOffset.y > 0 && 
				car_player->my_car.last_y > 
					car_player->my_car.car_y)
			{
				mapInfos->mapOffset.y-= 
					nearbyint(car_player->
							my_car.last_y -
					car_player->my_car.car_y);
				
				if(mapInfos->mapOffset.y > mapInfos->maxHeight)
					mapInfos->mapOffset.y = mapInfos->maxHeight;

				if(mapInfos->mapOffset.y < 0)
					mapInfos->mapOffset.y = 0;
			}

			if(car_player->my_car.car_y - mapInfos->mapOffset.y 
				> mapInfos->screen->h- (SCREEN_HEIGHT/2) &&
				mapInfos->mapOffset.y < mapInfos->maxHeight && 
				car_player->my_car.last_y < 
					car_player->my_car.car_y)
			{
				mapInfos->mapOffset.y += 
					nearbyint(car_player->
							my_car.car_y -
					car_player->my_car.last_y);
				
				if(mapInfos->mapOffset.y > mapInfos->maxHeight)
					mapInfos->mapOffset.y = mapInfos->maxHeight;

				if(mapInfos->mapOffset.y < 0)
					mapInfos->mapOffset.y = 0;
			}*/		
			
			
		}

		car_player = car_player -> next;
	
	}
}
