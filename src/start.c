#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "config.h"

#include "init.h"
#include "custom.h"
#include "mapdesigned.h"
#include "image.h"
#include "show.h"
#include "key.h"
#include "debug.h"
#include "car_move.h"
#include "main.h"
#include "car.h"
#include "text.h"
#include "offset.h"
#include "collision.h"
#include "math.h"
#include "pixel.h" 
#include "checkpoints.h"
#include "define.h"
#include "computer.h"

#define MAP_STATIC_COLLISION DATA_DIR"/maps/static/%s_collision.png"
#define MAP_STATIC_ROAD DATA_DIR"/maps/static/%s_road.png"

int init_load_game_data(
	SDL_Surface *screen, 
	char *selected_map,
	struct mapData *mapInfos,
	/*SDL_Surface **map_background, 
	SDL_Surface **map_checkpoints, */
	int type)
{
	if(type == MAP_TYPE_CUSTOM)
	{
		initCustomMapData(screen, 
				selected_map, 
				mapInfos);
				/*map_background, 
				map_checkpoints);*/
	}
	else
	{
	
	
		mapInfos->mpBg = load_background_map(
				screen, 
				selected_map);
		
		if(mapInfos->mpBg != NULL)
		{
		
			mapInfos->mpCp = load_checkpoints(
					screen, 
					selected_map);
			
			mapInfos->mpCollision = load_map_image(
					selected_map,
					MAP_STATIC_COLLISION);

			
			if(mapInfos->mpCollision != NULL)
			{
				SDL_SetColorKey(mapInfos->mpCollision,
					SDL_SRCCOLORKEY,
					SDL_MapRGB(mapInfos->mpCollision->format,
						255,255,255));

				mapInfos->cmap = sge_make_cmap(mapInfos->mpCollision);
			}
			
			mapInfos->mpRoad = load_map_image(
					selected_map,
					MAP_STATIC_ROAD);
			

			
		}
	}

        mapInfos->drivingdata = load_ai_data();

	if(mapInfos->mpCp != NULL)
		return(0);	
	
	return(-1);
	
}

int keyWaitHit()
{
	SDL_Event event;
	
		while(SDL_PollEvent(&event))
		{  
			switch(event.type)
			{  
				/* --- Here is keypress --- */
				case SDL_KEYDOWN:
				
					
				/*switch( event.key.keysym.sym )
				{
				}*/
				return(0);
				break;

				case SDL_QUIT:
				return(0);
				break;
			}
		}
	
	return(1);
		
}
/*
void showCarOnScreen(struct mapData *mapInfos)
{
	struct cars *p;

	p = mapInfos->p;

	while(p != NULL)
	{
		
		p = p->next;
	}
}
*/

void playersMove(struct mapData *mapInfos, int frameTime)
{
	struct cars *p;

	p = mapInfos->p;

	while(p != NULL)
	{
		if(p->my_car.computer)
		{
			computerIa(&(p->my_car), mapInfos);
		}
		
		keyCheckCommand( &(p->my_car), frameTime );
		
		p = p->next;
	}
}

/*Uint32 lookColor(SDL_Surface *s, int x, int y)
{
	Uint32 color = 0;

	color = getpixel(s, x, y);
	
	return(color);
}*/

int checkColorWithCheckpoint(
		Uint8 r, Uint8 g, Uint8 b, struct cars *car, 
		struct mapData *mapInfos)
{
	Uint8 oldColor;

	int colorStep = 8/*, colorRound = 32*/, rst = 0;
	/*
	if(car->my_car.finished == 1)
		return(5);
	*/


	if(r < 200 && r == b)
	{
		
		/*printf("r %d g %d b %d\n",r,g,b);*/

		oldColor = car->my_car.last_checkpoint_color;

		if((oldColor + colorStep + 1) == r)
			colorStep++;

		/*if(oldColor + colorStep == colorRound)
			colorStep++;*/

		if(oldColor + colorStep < mapInfos->totalCheckpoints)
		{
			if(oldColor + colorStep == r)
			{
				next_checkpoint(car, r);

				/*return(3);*/
			}
			else if(oldColor != r)
			{
				missed_checkpoint_return(car, mapInfos->screen);
			}
			else if(r == 0)
			{
				/*return(4);*/
			}
		}
		else if(r == 0)
		{
			next_checkpoint(car, r);

			if(car->my_car.finished == 0)
				car->my_car.num_lap++;
			
			rst = 2;
		}
		else if(oldColor != r)
		{
			missed_checkpoint_return(car, mapInfos->screen);
		}

		if(car->my_car.num_lap == mapInfos->maxTours && 
				car->my_car.finished == 0)
		{
			
			car->my_car.finished = 1;
			mapInfos->playersFinished++;
			car->my_car.position = mapInfos->playersFinished;
			
			if(CODEDEBUG)
				printf("Race ended for player %s\n",
						car->my_car.player_name);
			
			rst = 1;
			
		}
		
	}
	
	return(rst);
}
/*
void checkForCheckpoints(
		struct mapData *mapInfos, 
		struct cars *mc, 
		TTF_Font *font, 
		int hud, 
		int frameTime)
{
	float x1, y1, x2, y2, cos, sin,
	      ang, xDif, yDif;
	
	int ibcl = 0, rst, maxX, maxY, max;
	
	Uint8 r, g, b;

	Uint32 color, time;
	
	struct car_properties *car = &(mc->my_car);

	
	if(mapInfos->mpCp == NULL)
		return;
	
	x1 = (float)car->last_x;
	y1 = (float)car->last_y;
	
	x2 = (float)car->car_x;
	y2 = (float)car->car_y;

	xDif = fabs(x1-x2);
	yDif = fabs(y1-y2);

	if(xDif > 0.0001 && yDif > 0.0001)
	{

		ang = collisionGetAngle(x1, y1, x2, y2);

		ang /= ARAD;

		cos = cosf(ang);
		sin = sinf(ang);

		maxX = roundf(xDif / fabs(cos));
		maxY = roundf(yDif / fabs(sin));

		if(maxX > maxY)
			max = maxX;
		else
			max = maxY;
		
		if(max < 1)
			max = 1;

	}
	else
	{
		max = 1;
	}
*/

int getCheckColor(struct mapData *mapInfos,
		struct cars *p,
		int frameTime)
{

	Uint32 color;
	Uint8 r,g,b;

	int rst = 0, time;
	
		color = getpixel(mapInfos->mpCp, 
				(int)roundf(p->my_car.car_x), 
				(int)roundf(p->my_car.car_y));
		
		SDL_GetRGB(color, mapInfos->screen->format, 
				&r, &g, &b);
		
		rst = checkColorWithCheckpoint(r, g, b, p, 
				mapInfos);

		/*
		
		if(rst > 0)
		{
			if(rst < 5)
			{
				time = mapInfos->timeElapsed;
				
				p->my_car.lap_time[p->my_car.num_lap]
					= time	- frameTime + 
					((frameTime/max)*ibcl) - 
					mc->my_car.startTime;
				
				mc->my_car.startTime = time;
				
			}
			
		}*/
			
	

	return(rst);
	

}
	/*if(x1 > x2)
	{
		xmove = -1;
	}
	else
	{
		xmove = 1;
	}
	
	if(y1 > y2)
	{
		ymove = -1;
	}
	else
	{
		ymove = 1;
	}
	

	radcos = cos(ang/ARAD);
	radsin = sin(ang/ARAD);

	if(fabs(radcos) > 0.001)
	{
		fb = fabs(x1-x2) / radcos;
	
	

		
		while(bcl < fabs(fb) )
		{
			x1 += radcos;
			y1 += radsin;
		
			bcl++;

			

			
			rst = checkColorWithCheckpoint(r, g, b, mc, mapInfos);

		}
		
	
	}	*/
	/*
	if(car->last_x < car->car_x)
	{
		iBcl = car->last_x;
		iMax = car->car_x;
	}
	else
	{
		iBcl = car->car_x;
		iMax = car->last_x;
	}

	if(car->last_y < car->car_y)
	{
		jBcl = car->last_y;
		jMax = car->car_y;
	}
	else
	{
		jBcl = car->car_y;
		jMax = car->last_y;
	}
	
	while(iBcl < iMax)
	{
		while(jBcl < jMax)
		{
			printf("%.0lf - ",iBcl);
			jBcl++;
		}
		iBcl++;
	}
	printf("\n");
	*/


void updateCarPosition(struct mapData *mapInfos, 
		int frameTime, 
		TTF_Font *font, 
		int hud)
{
	struct cars *p;

	p = mapInfos->p;

	while(p != NULL)
	{
		move_advanced(mapInfos->mpBg, 
				&(p->my_car), 
				frameTime);
		
		
		p = p->next;
	}
}

void setAllCar(struct cars *p)
{
	while(p != NULL)
	{
		p->my_car.collision = 0;
		p = p->next;
	}
}

void moveAllCarOneFrame(struct cars *p)
{
	while(p != NULL)
	{
		if(p->my_car.collision == 0)
		{
			p->my_car.car_x += p->my_car.velx;
			p->my_car.car_y += p->my_car.vely;
		}

		p = p->next;
	}
}

void checkPointsNow(struct mapData *mapInfos, int millitime)
{
	struct cars *p = mapInfos->p;
	
	if(SDL_MUSTLOCK(mapInfos->mpCp))
		SDL_LockSurface(mapInfos->mpCp);

	while(p != NULL)
	{
		if(p->my_car.finished == 0)
		{
			getCheckColor(mapInfos,
				p, millitime);

		}

		p = p->next;
	}
	
	if(SDL_MUSTLOCK(mapInfos->mpCp))
		SDL_UnlockSurface(mapInfos->mpCp);
}


/*
void lookCheckpointStatus(
		struct mapData *mapInfos, 
		int frameTime, 
		TTF_Font *font, 
		int hud)
{
	struct cars *p;
	
	p = mapInfos->p;

	while(p != NULL)
	{

		if(p->my_car.finished == 0)
		{
			checkForCheckpoints(mapInfos, 
					p, 
					font, 
					hud, 
					frameTime);
		}
		
		p = p->next;
	}
}
*/
void checkCollisionMap(struct mapData *mapInfos, int frameTime)
{
	struct cars *p;

	Uint32 color, transparent, border;

	int ibcl, jbcl;
	
	if(mapInfos->mpCollision != NULL)
	{
		/*if(SDL_MUSTLOCK(mapInfos->mpCollision))
			SDL_LockSurface(mapInfos->mpCollision);
		*/
		p = mapInfos->p;
		
		while(p != NULL)
		{
			
			if(p->my_car.my_car_buffer != NULL)
			{
				if(1 == sge_cmcheck(
				p->my_car.cmap_surface,
				p->my_car.car_x-
				p->my_car.my_car_buffer->w/2,
				p->my_car.car_y-
				p->my_car.my_car_buffer->h/2,
				mapInfos->cmap, 0, 0) )
				{
					p->my_car.collision = 1;
				/*printf("%s collision map\n",
						p->my_car.player_name);*/
					/*if(p->my_car.speed > 0.2)
					{
						moveCar(&(p->my_car),
						p->my_car.last_x,
						p->my_car.last_y,
						mapInfos->mpBg);
					}*/
				}
				
				/*
				transparent = SDL_MapRGB(
						p->my_car.my_car_buffer->format,
						0,252,255);

				if(SDL_MUSTLOCK(p->my_car.my_car_buffer))
					SDL_LockSurface(p->my_car.my_car_buffer);

				for(ibcl = 0; 
					ibcl < p->my_car.my_car_buffer->w; 
					ibcl++)
				{
					for(jbcl = 0; 
						jbcl < p->my_car.my_car_buffer->h; 
						jbcl++)
					{
						color = getpixel(
							p->my_car.my_car_buffer, 
							ibcl, 
							jbcl);

						if(color != transparent )
						{
							border = getpixel(
								mapInfos->mpCollision,
								p->my_car.car_x,
								p->my_car.car_y);

							if(border == 0)
								printf("TO-DO:move\n");
								
						}
					}
				}
				

				if(SDL_MUSTLOCK(p->my_car.my_car_buffer))
					SDL_LockSurface(p->my_car.my_car_buffer);*/
			}
			
			p = p->next;
		}
		
		/*if(SDL_MUSTLOCK(mapInfos->mpCollision))
			SDL_LockSurface(mapInfos->mpCollision);*/
	}
}

void checkCollisionBetweenCars(struct mapData *mapInfos, int frameTime)
{
	struct cars *temp_cars = NULL, *car_player;
	
	car_player = mapInfos->p;
	
	while(car_player != NULL)
	{
		
		temp_cars = car_player->next;

		while(temp_cars != NULL)
		{
			/*printf("check collision between %s and %s \n",car_player->my_car.player_name, temp_cars->my_car.player_name);*/
			if(car_player != temp_cars && 
			car_player -> my_car.cmap_surface != NULL && 
				temp_cars->my_car.cmap_surface != NULL)
			{
				if(1 == sge_cmcheck(
				car_player->my_car.cmap_surface,
				car_player->my_car.car_x -
				car_player->my_car.car->w/2,
				car_player->my_car.car_y -
				car_player->my_car.car->h/2,
				
				temp_cars->my_car.cmap_surface,
				temp_cars->my_car.car_x-
				temp_cars->my_car.car->w/2,
				temp_cars->my_car.car_y-
				temp_cars->my_car.car->h/2) )
				{
					carCollision(car_player, 
						temp_cars,
						mapInfos->mpBg,
						frameTime);
				}
			}
				
			temp_cars = temp_cars->next;
		}

		car_player = car_player->next;
	}
}

void updateByFrame(
		struct mapData *mapInfos, 
		int frameTime, 
		TTF_Font *font, 
		int hud)
{
	int bcl, max = frameTime;

	if(max < 1)
		max = 1;

	setAllCar(mapInfos->p);

	for(bcl = 0; bcl < max; bcl++)
	{
		moveAllCarOneFrame(mapInfos->p);
		
		checkCollisionBetweenCars(mapInfos, 
				frameTime);
			
		checkCollisionMap(mapInfos, 
				frameTime);

		carOnEdge(mapInfos->mpBg, 
				mapInfos->p,
				frameTime);


		checkPointsNow(mapInfos, frameTime);
	}
}

void initStarTime(struct mapData *mapInfos)
{
	struct cars *p;

	Uint32 time = mapInfos->timeElapsed; /* SDL_GetTicks();*/

	p = mapInfos->p;

	while(p!= NULL)
	{
		p->my_car.startTime = time;

		p = p->next;
	}
}

void blitEverything(struct mapData *mapInfos)
{
	SDL_Rect dest;

	struct cars *p;

	int bcl;

	Uint32 color = SDL_MapRGB(mapInfos->screen->format, 
			0,0,0);

	/* Prepare each car img - Should not be done for car out screen */
	p = mapInfos->p;

	while(p != NULL)
	{
		rotateCar(mapInfos, p);
		p = p->next;
	}

	/* Blit everything for each screen view */
	
	p = mapInfos->p;

	while(p != NULL)
	{
		if(p->my_car.computer == 0)
		{
			dest = p->my_car.destView;

			
			/*dest.x = 0;
			dest.y = 0;*/
			/* Layer 1 */
			/*
			printf("blit background ");
			printf("offset x y %d %d - ",
					p->my_car.offsetView.x,
					p->my_car.offsetView.y);
			printf("w h %d %d - ",
					p->my_car.destView.w,
					p->my_car.destView.h);
			printf("on %d %d\n",
					p->my_car.destView.x,
					p->my_car.destView.y);
			
			printf("offset w h %d %d\n",
					p->my_car.offsetView.w,
					p->my_car.offsetView.h);
			*/
			SDL_BlitSurface(
				mapInfos->mpBg, 
				&(p->my_car.offsetView), 
				mapInfos->screen, 
				&dest);
			
			/* Layer 2 */
			showCarOnScreen(mapInfos, p);
			
			/* Layer 3 */
			/*if(mapInfos->mpDecoration != NULL)
				SDL_BlitSurface(mapInfos->mpDecoration, 
						&(p->my_car.offsetView), 
						mapInfos->screen, 
						&dest);*/
			
			if(dest.y > 0 && dest.y < mapInfos->screen->h)
			{
				if(SDL_MUSTLOCK(mapInfos->screen))
					SDL_LockSurface(mapInfos->screen);
				
				for(bcl =0; bcl < mapInfos->screen->w; bcl++)
				{
					putpixel(mapInfos->screen,
							bcl, 
							dest.y,
							color);
				}

				if(SDL_MUSTLOCK(mapInfos->screen))
					SDL_UnlockSurface(mapInfos->screen);
			}
		}
		p = p->next;
	}
}

void startRace(SDL_Joystick *joystick1, TTF_Font *font, struct mapData *mapInfos)
{
	int time1, time2, loop = 0, 
	    bcl = 0, fps = 0, tstart, 
	    tend, seconds = 0, gameStarted = 0, 
	    whatLight = 3, pause = 0, debugIa = 0, 
	    frameTime = 0, hud = 2, maxTours = 3;
	SDL_Surface *sLight = NULL;

			

	time1 = SDL_GetTicks();
	
	time2 = time1;

	tstart = time2;
	
	do
	{
		
		
		if(gameStarted && !pause)
		{
			mapInfos->timeElapsed += frameTime;

			playersMove(mapInfos, frameTime);

			updateCarPosition(mapInfos, 
					frameTime, 
					font, 
					hud);

			updateByFrame(mapInfos, frameTime,font,hud);
			/*
			carOnEdge(mapInfos->mpBg, 
				mapInfos->p, 
				frameTime);

			checkCollisionBetweenCars(mapInfos, 
				frameTime);

			checkCollisionMap(mapInfos, 
				frameTime);
			
			lookCheckpointStatus(mapInfos, 
				frameTime, 
				font, 
				hud);
			*/
			
			updateMapOffset(mapInfos);
		}
		
		/*keyPlayersHit(mapsInfos->p, pause, game_started, key_fct_sdlcode, SDL_GetTicks()-time1);
		
		keyLoopEvent(mapInfos->p, key_fct_sdlcode, game_started, &pause, &debug_ia, &loop);

		if (game_started == 1 && pause == 0)
		{
			moveAllCar(
				surface, 
				background, 
				&map_offset, 
				map_decal_max_x, 
				map_decal_max_y,
				rotation_angle,
				mapInfos->p,
				SDL_GetTicks() - time1 );
		}
		
		
		carOnEdge(background, mapInfos->p);
		

		handleRace(
				surface, 
				background, 
				checkpoints, 
				mapInfos->p, 
				tot_checkpoints, 
				racetype, 
				&map_offset,
				game_started,
				time_seconds,
				&endgame,
				max_tours,
				finished,
				SDL_GetTicks() - time1);

				*/
		
		/* Layer 1, optional; if not available, the mpBg will be used. */
		/*if(mapInfos->mpRoad != NULL)
			SDL_BlitSurface(mapInfos->mpRoad, &(mapInfos->mapOffset), mapInfos->screen, NULL);
		else*/
		/* basic map image composed by green (grass) and grey (road), this is necessary for the ia */

		blitEverything(mapInfos);

		
		if(gameStarted == 0 || (gameStarted == 1 && seconds < 2) )
		{
			showStartLight(
				mapInfos->screen,
				whatLight,
				&sLight);
		
			blitStartLight(mapInfos, sLight);
			
		}
		
		if(hud)
		{
			show_hud(mapInfos->screen, 
				font, 
				mapInfos->p, 
				pause, 
				mapInfos->maxTours, 
				fps, 
				hud, 
				mapInfos);
		}
		

		/*SDL_UpdateRect(mapInfos->screen, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);*/
		
		SDL_Flip(mapInfos->screen);
		
		bcl ++;
		
		time2 = SDL_GetTicks();
		
		tend = time2;

		frameTime = tend-tstart;
	
		/*if(gameStarted && !pause)*/
		/*SDL_Delay( INTERVAL-frameTime );*/
		
		/* tend - tstart  = time duration of the current frame */
		tstart = tend;
		
		
		if( (time2-time1)>=1000)
		{
			fps = bcl; 
			
			if(CODEDEBUG)
				printf("FPS: %d\n",fps);
			
			if(!pause)
			{
				seconds++;
	
				if(sLight != NULL)
				{
					SDL_FreeSurface(sLight);
					sLight = NULL;
				}
			

			/*printf("%d\n",seconds);*/

				if(gameStarted == 0)
				{
					if(seconds == 3)
					{
						gameStarted = 1;
						seconds = 0;
						whatLight = 0;
						initStarTime(mapInfos);
					}
					else
						whatLight = 3 - seconds;
		
				}
			}
			
			if(CODEDEBUG)
				printf("map offset: %d, %d\n",
						mapInfos->mapOffset.x, 
						mapInfos->mapOffset.y);
			
			bcl = 0 ;
			
			time1 = time2;

		}
		
		loop = keyGetEvents(
				gameStarted, 
				&pause, 
				&debugIa
				/*&(mapInfos->p->next->my_car.computer)*/, 
				&hud,
				mapInfos);
		
		
	} while(loop == 1);

}
		
		
/*void init_load_cars(
	SDL_Surface *screen,
	int *p_players_number)
{
	int nplayers;
	
	if(p_players_number == NULL)
		return;
	
	nplayers = *p_players_number;
	
	
}

void startCustomMap(
	SDL_Surface *screen, 
	char *selected_map_name,
	SDL_Surface **map_background,
	SDL_Surface **map_checkpoints,
	int *p_players_number,
	SDL_Joystick *the_joy,
	TTF_Font* font,
	char *selected_car_name)
{
	initCustomMapData(
		screen, 
		selected_map_name, 
		map_background, 
		map_checkpoints);
	
	if(*map_checkpoints != NULL)
	{
		init_load_cars(
			screen,
			p_players_number);

		init_all(
			screen, 
			the_joy,
			font,
			*map_background,
			*map_checkpoints,
			selected_map_name,
			selected_car_name);

		SDL_FreeSurface(*map_checkpoints);
		*map_checkpoints = NULL;
		
	}
	if(*map_background != NULL)
	{
		SDL_FreeSurface(*map_background);
		*map_background = NULL;
	}
}

void launch_a_game(
	SDL_Surface *screen, 
	char *selected_map_name,
	SDL_Surface **map_background,
	SDL_Surface **map_checkpoints,
	int *p_players_number,
	SDL_Joystick *the_joy,
	TTF_Font* font,
	char *selected_car_name)
{
	init_load_game_data(
		screen, 
		selected_map_name, 
		map_background, 
		map_checkpoints);

	if(*map_checkpoints != NULL)
	{
		init_load_cars(
			screen,
			p_players_number);
		
		init_all(
			screen, 
			the_joy,
			font,
			*map_background,
			*map_checkpoints,
			selected_map_name,
			selected_car_name);
		
		SDL_FreeSurface(*map_checkpoints);
		
		*map_checkpoints = NULL;
	}
	
	if(*map_background != NULL)
	{
		SDL_FreeSurface(*map_background);
		
		*map_background = NULL;
	}

}*/
