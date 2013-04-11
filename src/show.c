#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/sge.h>

#include "image.h"
#include "map_struct.h"
#include "struct_car.h"
#include "debug.h"


void rotateCar(struct mapData *mapInfos,
	      struct cars *car_player)
{
	if(car_player->my_car.my_car_buffer == NULL)
	{
		fprintf(stderr, "Error, car_buffer is NULL\n");
		return;
		/*car_player = car_player->next;
		continue;*/
	}
	if(car_player->my_car.car == NULL)
	{
		fprintf(stderr, "Error, car is NULL\n");
		return;
		/*car_player = car_player->next;
		continue;*/
	}
	
	if(CODEDEBUG)
		printf(" Initializing car img data\n");

	
	SDL_FillRect(car_player->my_car.my_car_buffer,
			NULL,
			SDL_MapRGB(
				mapInfos->screen->format,
				0,252,255) );
	
	sge_transform(
		car_player->my_car.car, 
		car_player->my_car.my_car_buffer, 
		car_player->my_car.rotation, 
		1.0,
		1.0,
		car_player->my_car.car->w/2, 
		car_player->my_car.car->h/2, 
		car_player->my_car.my_car_buffer->w/2,
		car_player->my_car.my_car_buffer->h/2,
		SGE_TAA | SGE_TSAFE );
	
	
	SDL_SetColorKey(
		car_player->my_car.my_car_buffer, 
		SDL_SRCCOLORKEY, 
		SDL_MapRGB(
			mapInfos->screen->format, 
			0, 
			252, 
			255)
		);
	
	if(car_player -> my_car.cmap_surface != NULL)
		sge_destroy_cmap(car_player -> my_car.cmap_surface);
	
	car_player->my_car.cmap_surface = 
		sge_make_cmap(car_player->my_car.my_car_buffer);
	
	if(CODEDEBUG)
	{
		printf(" The drawing has been rotated for this car\n");
	}
}

void showCarOnScreen(struct mapData *mapInfos,
		struct cars *p)
{
	SDL_Rect dest;

	struct cars *car_player = mapInfos->p;

	while(car_player != NULL)
	{


		dest.w = car_player->my_car.my_car_buffer->w;
		dest.h = car_player->my_car.my_car_buffer->h;
		
		dest.x = car_player->my_car.car_x - 
			p->my_car.offsetView.x - 
			car_player->my_car.my_car_buffer->w/2;
		dest.y = car_player->my_car.car_y - 
			p->my_car.offsetView.y - 
			car_player->my_car.my_car_buffer->h/2;
		
		copy_car_toscreen(
			car_player->my_car.my_car_buffer,
			mapInfos->screen, 
			&dest,
			p);
		
		car_player = car_player-> next;
	}
}
