#include <stdio.h>
#include <SDL/SDL.h>

#include "struct_car.h"
#include "map_struct.h"
#include "text.h"

extern const double ADD_SPEED;


void next_checkpoint(struct cars *car_player, int color)
{
	if(car_player == NULL)
		return;

	/*if(color == 0)
	{
		car_player -> my_car.num_lap++;
	
		car_player->my_car.startTime = SDL_GetTicks();
	}*/
	/*if(car_player->my_car.last_checkpoint_color != color)
		printf("Checkpoint %d\n", color);*/

	car_player -> 
		my_car.last_checkpoint_color = color;
	
	car_player -> my_car.last_checkpoint_x =
		car_player -> my_car.car_x;
	
	car_player -> my_car.last_checkpoint_y =
		car_player -> my_car.car_y;
	
	car_player -> my_car.
		last_checkpoint_rotation = 
		car_player -> my_car.rotation;

	car_player -> my_car.missed = 0;

}

void missed_checkpoint_return(struct cars *car_player, SDL_Surface *screen)
{

	
	if(car_player == NULL)
		return;
	

	if(car_player -> my_car.computer == 0)
	{

		car_player -> my_car.missed = 1;

		/*printf("TO-DO: draw_frame - you missed a checkpoint !!!\n");*/
		return;
	}
	
	/*printf("PRINTME: updating the whole screen (missed checkp)\n");*/
	
	/*SDL_Flip(screen);*/
	
	car_player -> my_car.last_x = 
		car_player -> my_car.car_x;
	car_player -> my_car.last_y =
		car_player -> my_car.car_y;

	car_player -> my_car.speed = ADD_SPEED;


	car_player -> my_car.car_x = car_player -> my_car.last_checkpoint_x;
	car_player -> my_car.car_y = car_player -> my_car.last_checkpoint_y;
		
	car_player -> my_car.rotation = 
		car_player -> my_car.last_checkpoint_rotation;

}

