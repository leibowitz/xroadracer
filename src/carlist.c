#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "config.h"

#include "struct_car.h"
#include "mask.h"
#include "debug.h"
#include "key.h"
#include "main.h"
#include "image.h"
#include "define.h"

extern const double MAX_SPEED_ROAD;
extern const double ADD_SPEED_ROAD;

#define IMGCAR1 DATA_DIR"/cars/ferrariyellow.gif"
#define IMGCAR2 DATA_DIR"/cars/porschegreen.gif"

void carlistClear(struct cars **top)
{
	struct cars *the_cars = NULL, *temp_cars = NULL;

	int bcl = 1;

	if(top == NULL)
		return;
	
	the_cars = *top;

	if(CODEDEBUG)
		printf(" - carlistClear() : clear the whole car list allocated.\n");

	
	while(the_cars != NULL)
	{
		if(CODEDEBUG)
			printf(" -%d- One car found. \n", bcl);
		
		the_cars -> prev = NULL;
		temp_cars = the_cars -> next;
		
		
		if(the_cars->my_car.car != NULL)
		{
			SDL_FreeSurface(the_cars->my_car.car);
			
			the_cars->my_car.car = NULL;

			if(CODEDEBUG)
				printf(" 1/3 car surface cleared\n");
		}
		
		if(the_cars -> my_car.my_car_buffer != NULL)
		{
			SDL_FreeSurface(the_cars -> my_car.my_car_buffer);
			
			the_cars -> my_car.my_car_buffer = NULL;

			if(CODEDEBUG)
				printf(" 2/3 car buffer surface cleared\n");
		}
		
		if(the_cars -> my_car.cmap_surface != NULL)
		{
			sge_destroy_cmap(the_cars -> my_car.cmap_surface);

			if(CODEDEBUG)
				printf(" 3/3 sge surface cleared\n");
		}
		
		free (the_cars);
		
		the_cars = temp_cars;

		if(CODEDEBUG)
			printf(" -%d- The car memory has been successfully cleared.\n", bcl);
		
		bcl++;
	}

	if(top != NULL)
		*top = NULL;

}

int init_car_list(SDL_Surface *surface, struct cars *list_cars)
{
	struct cars *car_player = NULL;
		/*, *computer_player = NULL;*/
	
	SDL_Surface *img_temp = NULL, *image = NULL;

	int nb = 0,bcl, computer = 0, human = 0,
	    viewHeight, viewWidth;

	char *car_img = NULL, carHuman[] = IMGCAR1, 
	     carComputer[] = IMGCAR2;
	
	/******************/
	
	/*if(list_cars == NULL)
		return(-1);*/
	
	/******************/

	if(CODEDEBUG)
		printf(" - init_car_list() : Set the critical variables for each car in the car list.\n");
	
	car_player = list_cars;
	
	while(car_player != NULL)
	{
		car_player->my_car.playerNumber = nb;
		car_player->my_car.finished = 0;

		if(car_player -> prev != NULL)
		{
			
			car_player -> my_car.computer = 1;
			sprintf(car_player -> my_car.player_name,
					"Computer %d",
					(computer+1));

			car_player -> my_car.k.keys[KEY_MOVEUP] = 0;
			car_player -> my_car.k.keys[KEY_MOVERIGHT] = 0;
			car_player -> my_car.k.keys[KEY_MOVEDOWN] = 0;
			car_player -> my_car.k.keys[KEY_MOVELEFT] = 0;
			
			computer++;
			
			/*
			car_player->my_car.computer = 0;
			sprintf(car_player->my_car.player_name,
				"Player %d",
				(human+1));
			human++;
			
			car_player -> my_car.k.keys[KEY_MOVEUP] = SDLK_z;
			car_player -> my_car.k.keys[KEY_MOVERIGHT] = SDLK_d;
			car_player -> my_car.k.keys[KEY_MOVEDOWN] = SDLK_s;
			car_player -> my_car.k.keys[KEY_MOVELEFT] = SDLK_q;
			*/
			/* end here */
			car_img = carComputer;

		}
		else
		{
			car_player -> my_car.computer = 0;
			sprintf(car_player -> my_car.player_name,
					"Player %d",
					(human+1) );
			human++;
			
			car_player -> my_car.k.keys[KEY_MOVEUP] = SDLK_UP;
			car_player -> my_car.k.keys[KEY_MOVERIGHT] = SDLK_RIGHT;
			car_player -> my_car.k.keys[KEY_MOVEDOWN] = SDLK_DOWN;
			car_player -> my_car.k.keys[KEY_MOVELEFT] = SDLK_LEFT;

			car_img = carHuman;

		}

					
		image = Load_img_from(car_img);
		
		if(image == NULL)
		{
			fprintf(stderr, 
				"Error loading car img\n");
			/*break;*/
		}
		else
		/*if(image != NULL)*/
		{
			car_player->my_car.car = 
				SDL_DisplayFormatAlpha(image);
			
			SDL_FreeSurface(image);
			
			image = NULL;

			if(car_player -> my_car.car == NULL)
			{
				fprintf(stderr, 
				"Error allocating car img\n.");
				/*break;*/
			}
			else
			if(CODEDEBUG)
			{
				printf("The car img has been successfully ");
				printf("loaded and initialized.\n");
			}
		}

		car_player -> my_car.car_width = 20;
		car_player -> my_car.car_height = 20;

		/*car_player -> my_car.car = NULL;
		car_player -> my_car.my_car_buffer  = NULL;
		car_player -> my_car.cmap_surface  = NULL;*/
		
		if(CODEDEBUG)
			printf(" -%d- One car found.\n", (nb+1) );
		/*SDL_SetColorKey(car_player->my_car.car, 
			SDL_SRCCOLORKEY, 
			SDL_MapRGB(car_player->my_car.car->format,
				0, 252, 255));
		img_temp = SDL_CreateRGBSurface(
			SDL_SWSURFACE, 
			car_player -> my_car.car_width*3, 
			car_player -> my_car.car_height*3, 
			16, 
			rmask, gmask, bmask, amask);
		if(img_temp == NULL)
		{
			fprintf(stderr, "Error creating second rgb surface\n");
			return;
		}
		car_player->my_car.my_car_buffer = SDL_DisplayFormat(img_temp);
		if(car_player->my_car.my_car_buffer == NULL)
		{
			fprintf(stderr, "Error copying temp surface\n");
			return;
		}
		SDL_FreeSurface(img_temp);
		img_temp = car_player -> my_car.my_car_buffer;
		if(0 != SDL_FillRect(img_temp,
			NULL, SDL_MapRGB(surface->format, 0, 252, 255)) )
		{
			fprintf(stderr,"Error filling color in 2\n");
			return;
		}
		
		SDL_BlitSurface(car_player->my_car.car, NULL,
			img_temp, NULL);
		
		
		SDL_FreeSurface(car_player -> my_car.car);
		car_player -> my_car.car = img_temp;
		*/
		/* creating the buffer */
			
		car_player -> my_car.my_car_buffer = SDL_CreateRGBSurface(
			SDL_SWSURFACE /*SDL_SRCCOLORKEY*/, 
			car_player -> my_car.car_width*3, 
			car_player -> my_car.car_height*3, 
			16, 
			rmask, gmask, bmask, amask);
		
		if(car_player -> my_car.my_car_buffer == NULL)
		{
			fprintf(stderr, "CreateRGBSurface failed: %s\n", 
				SDL_GetError());
			return(-1);
		}

		if(CODEDEBUG)
			printf(" 1/4 - Car buffer surface created.\n");
		
		img_temp = SDL_DisplayFormat(car_player -> my_car.my_car_buffer);
		
		SDL_FreeSurface(car_player -> my_car.my_car_buffer);
		
		car_player -> my_car.my_car_buffer = img_temp;
		
		if(img_temp == NULL)
		{
			fprintf(stderr, "Error copying img_temp surface\n");
			return(-1);
		}

		if(CODEDEBUG)
			printf(" 2/4 - Surface transformed successfully.\n");
		
		if(0 != SDL_FillRect(car_player -> my_car.my_car_buffer,
			NULL, SDL_MapRGB(surface->format, 0, 252, 255)) )
		{
			fprintf(stderr,"Error filling color\n");
			return(-1);
		}

		if(CODEDEBUG)
			printf(" 3/4 - Filled with color.\n");

		if(car_player->my_car.car != NULL)
		{
				
			SDL_SetColorKey(car_player->my_car.car, 
				SDL_SRCCOLORKEY, 
				SDL_MapRGB(surface->format,
					0, 252, 255));
		}
		else
			fprintf(stderr, "Error with car surface, please restart the game.\n");
		
		if(CODEDEBUG)
			printf(" 4/4 - color key set.\n");

		car_player -> my_car.speed = 0.0;
		
		car_player-> my_car.SPEED_MAX = MAX_SPEED_ROAD;

		if(car_player -> my_car.computer)
			car_player->my_car.SPEED_MAX += 1.4;
		
		car_player-> my_car.ADD_SPEED = ADD_SPEED_ROAD;
		
		
		car_player -> my_car.xvel = 0;
		car_player -> my_car.yvel = 0;
		car_player -> my_car.is_grass = 0;
		car_player -> my_car.grass_detection = 0;
		car_player -> my_car.rotation_side = 0;
		car_player -> my_car.turning = 0;
		car_player -> my_car.time_penality = 0;
		
		car_player -> my_car.last_checkpoint_color = 0;
		
		car_player -> my_car.last_elapsed = 0;
		for(bcl =0; bcl < MAX_LAPS; bcl++)
		{
			car_player -> my_car.lap_time[bcl] = 0;
		}
		car_player -> my_car.startTime = 0;
		car_player -> my_car.total_time = 0;
		car_player -> my_car.num_lap = 0;
		car_player -> my_car.position = 0;

		/*computer_player = car_player ; */
		
		/*printf("%lf %lf\n",car_player -> my_car.car_x,
			car_player -> my_car.car_y);
		*/
		nb++;
		
		/*if(car_player -> my_car.car == NULL)
		{
			fprintf(stderr, "Car not set up correctly, unable to continue\n");
			return(nb);
		}*/
		
		car_player = car_player -> next;
	}
	
	car_player = list_cars;

	bcl = 0;
	
	if(nb > 0)
	{
		if(human == 0)
			nb = computer;
		else
			nb = human;
		
		/* Split the screen in horizontal */
		viewHeight = surface->h/nb;
		
		viewWidth = surface->w;
		
		while(car_player != NULL)
		{
			if(car_player->my_car.computer == 0)
			{
				car_player->my_car.destView.x = 0;
				
				car_player->my_car.destView.y = bcl*viewHeight;

				car_player->my_car.destView.h = 
					car_player->my_car.offsetView.h = viewHeight;

				car_player->my_car.destView.w =
					car_player->my_car.offsetView.w = viewWidth;
				/*

				printf("%s - %d %d %d %d\n",
						car_player->my_car.player_name,
						car_player->my_car.destView.x,
						car_player->my_car.destView.y,
						car_player->my_car.destView.w,
						car_player->my_car.destView.h);
				*/
				bcl++;
			}
			else
			{
				car_player->my_car.destView.x = 
					car_player->my_car.destView.y =
					car_player->my_car.destView.w = 
					car_player->my_car.destView.h = 0;
			}
			
			car_player = car_player -> next;

		}

		nb = human + computer;
	}
	
	return ( nb );
}

void setNewCarVar(struct cars *car)
{
	car -> my_car.car = NULL;
	car -> my_car.my_car_buffer = NULL;
	car -> my_car.cmap_surface = NULL;

	car-> my_car.missed = 0;
	car->my_car.maxD = 0;
	car->my_car.debugAi = 0;
	car->my_car.trainAi = 0;
	car->my_car.collision = 0;
}

struct cars * add_car(struct cars **list_cars)
{
	struct cars *new_car = NULL, *temp_car = NULL;
	
	if(list_cars == NULL)
		return(NULL);
		
	new_car = (struct cars *) malloc( sizeof(struct cars) );
	
	if(new_car == NULL)
	{
		fprintf(stderr, "Error: unable to allocate memory\n");
		return(NULL);
	}
	
	new_car -> next = NULL;

	setNewCarVar(new_car);
	
	if(*list_cars == NULL)
	{
		if(CODEDEBUG)
			printf(" - add_car() - Adding the first car to the list\n");
		
		*list_cars = new_car;
		new_car -> prev = NULL;
		return(new_car);
	}
	else
	{
		temp_car = *list_cars; 
	
		while(temp_car -> next != NULL)
		{
			temp_car = temp_car -> next;
		}

		if(CODEDEBUG)
			printf(" - add_car() : putting the new car to the end of the list\n");
		
		temp_car -> next = new_car;
		new_car  -> prev = temp_car;
	}
	
	return(new_car);
}

void clear_cars(
	SDL_Surface *screen, 
	SDL_Surface *background, 
	struct cars *the_player)
{
	SDL_Rect dest;
	struct cars *car_player = the_player;
	
	while(car_player != NULL)
	{
		dest.x = 0;
		dest.y = 0;
		
		dest.w = 1;
		dest.h = 1;
		
		if(background != NULL)
		{
			/*printf("PRINTME: start_game.c - clear background x %d y %d\n",
					(int)car_player->my_car.last_x,
					(int)car_player->my_car.last_y);
			*/
			dest.x = (car_player->my_car.last_x)-
				car_player->my_car.my_car_buffer->w;
			dest.y = (car_player->my_car.last_y)-
				car_player->my_car.my_car_buffer->h;
			
			dest.w = car_player->my_car.my_car_buffer->w*2;
			dest.h = car_player->my_car.my_car_buffer->h*2;
			
			if((int)dest.x < 0)
				dest.x = 0;
			else if((int)dest.x < screen->w && 
			(int)dest.x+dest.w > screen->w)
			{
				dest.w = screen->w - (int)dest.x;
			}
			if((int)dest.y < 0)
				dest.y = 0;
			else if((int)dest.y < screen->h && 
				(int)dest.y+dest.h > screen->h)
			{
				dest.h = screen->h - (int)dest.y;
			}
			SDL_BlitSurface(background, &dest,
			screen, &dest);

			/*if(missed_checkpoint == 1)
				SDL_UpdateRect(screen, 
				(int)dest.x, 
				(int)dest.y,
				(int)dest.w,
				(int)dest.h);
			*/
		}
		car_player = car_player->next;
	}
}
