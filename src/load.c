#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "config.h"

#include "debug.h"
#include "fill.h"
#include "menu.h"
#include "start.h"
#include "image.h"
#include "text.h"
#include "carlist.h"
#include "map_struct.h"
#include "read.h"
#include "define.h"
#include "key.h"
#include "hud.h"
#include "custom.h"

#define BACKGROUND_IMG DATA_DIR"/images/menu_background_color.gif"
#define BUTTON_IMG DATA_DIR"/images/bouton-3.gif"
#define SPLASH_IMG DATA_DIR"/images/xrr_splash.png"

void loadMenuImg(SDL_Surface **back, SDL_Surface **selector)
{
	*back = surface_from_img(BACKGROUND_IMG);
	/**back = surface_from_img("images/xrr_splash.png");

	*selector = surface_from_img("images/menu_selection.gif");*/
	*selector = surface_from_img(BUTTON_IMG);

	if(*selector != NULL)
	{
		SDL_SetColorKey(*selector,
				SDL_SRCCOLORKEY,
				SDL_MapRGB((*selector)->format,
				0,0,255));

		SDL_SetAlpha(*selector,
				SDL_SRCALPHA,
				210);
	}
	
}

/*void startGameMenu_Principal(SDL_Surface *screen,
		TTF_Font *font,
		SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack,
		SDL_Surface *menuImgSelector)
{
	char map_name[250] = "simsilver\0", 
		*selected_map_name = NULL;
	
	char car_name[250] = "\0", 
		*selected_car_name = NULL;
	
	int 
		nplayers = 2;
	
	int status = 0, rmenu = 0;

	while(status >= 0)
	{
		fillSurfaceWithBlack(screen);

		status = menuShow_Principal(screen, font, joystick1, 
		menuImgBack, menuImgSelector);
		
		selected_map_name = map_name;
		
		selected_car_name = car_name;
		
		
		status = show_menu(
			screen, 
			&nplayers,
			font);
		
		rmenu = 0;
		

		
		switch(status)
		{
			case MENU_PLAY:
			rmenu = show_play_menu(
				screen,
				font);
			
			
			if(rmenu > -1)
			{
				loadRace(
					screen, 
					map_name,
					&nplayers,
					joystick1,
					font,
					selected_car_name);
			}
			break;
			
			case MENU_CUSTOMMAP:
			rmenu = show_custommap_menu(
				screen, 
				map_name, 
				font);
			
			if(rmenu > -1)
				startCustomMap(
					screen, 
					selected_map_name,
					&map_background,
					&map_checkpoints,
					p_players_number,
					joystick1,
					font,
					selected_car_name);
			break;

			case MENU_QUIT:
			break;
		}
	}
}*/

void menuImgClear(SDL_Surface ** s)
{
	if(*s != NULL)
	{
		SDL_FreeSurface(*s);

		*s = NULL;
	}

}

void clearMapData(struct mapData **mapInfos)
{
	if(mapInfos == NULL)
		return;
	
	if(CODEDEBUG)
		printf(" - clearMapData() : Clear memory for the mapData structure. \n");

	if(*mapInfos != NULL)
	{
		menuImgClear( &((*mapInfos)->mpBg) );

		menuImgClear( &((*mapInfos)->mpCp) );

		menuImgClear( &((*mapInfos)->mpRoad) );
		
		menuImgClear( &((*mapInfos)->mpDecoration) );

		menuImgClear( &((*mapInfos)->mpCollision) );

		menuImgClear( &((*mapInfos)->hudSpeed) );
		
		menuImgClear( &((*mapInfos)->hudLaps) );
			
		if(CODEDEBUG)
			printf(" 1/3 : All Surface cleared\n");
		
		if((*mapInfos)->p != NULL)
		{
			/*clear cars*/
			carlistClear( &((*mapInfos)->p) );
			if(CODEDEBUG)
				printf(" 2/3 : Car list cleared\n");
		}

		if((*mapInfos)->longName != NULL)
		{
			/*free((*mapInfos)->longName);
			(*mapInfos)->longName = NULL;*/
		}

		if((*mapInfos)->cmap != NULL)
			sge_destroy_cmap((*mapInfos)->cmap);
		
		if(CODEDEBUG)
			printf(" 3/3 : Clearing mapInfos structure allocation\n");
		
                clear_ai_data((*mapInfos)->drivingdata);
		free(*mapInfos);
		*mapInfos = NULL;
	}
	
	if(CODEDEBUG)
		printf(" - clearMapData() : ended correctly.\n");
}

/*void loadAllMapImg(SDL_Surface *screen, struct mapData *mapInfos, char *mapName)
{
	init_load_game_data(screen, mapName, &(mapInfos->mpBg), &(mapInfos->mpCp) );
}*/

void checkCarData(struct mapData *mapInfos)
{
	struct cars *listCars = NULL;
	int bcl = 1;
			
	if(CODEDEBUG)
		printf(" - checkCarData() : count how many cars allocated.\n");
	
	if(mapInfos != NULL)
	{
		listCars = mapInfos->p;

		while(listCars != NULL)
		{
			if(CODEDEBUG)
			{
				printf(" car found - %d\n", bcl );
				if(listCars->my_car.car == NULL)
					printf(" Error, car surface is NULL\n");
			}
			
			bcl++;
			
			listCars = listCars->next;
		}
	}


	if(CODEDEBUG)
		printf(" - checkCarData() : ended correctly.\n");
}

void loadMoreMapImg(struct mapData *mapInfos)
{
	Uint32 color = SDL_MapRGB(mapInfos->screen->format,
					0,0,255);
	/* Load road and decoration img */
	mapInfos->mpDecoration = 
		customLoadSurface(
				mapInfos->screen,
				(int)mapInfos->mpBg->w,
				(int)mapInfos->mpBg->h,
				color
			);

	if(mapInfos->mpDecoration != NULL)
	{
		SDL_SetColorKey(mapInfos->mpDecoration,
				SDL_SRCCOLORKEY,
				color);
	}

}

void checkViewOffset(struct cars *p)
{
	if(p->my_car.offsetView.x < 0)
		p->my_car.offsetView.x = 0;

	if(p->my_car.offsetView.y < 0)
		p->my_car.offsetView.y = 0;
	
	if(p->my_car.offsetView.x > 
			p->my_car.maxWidthView)
		p->my_car.offsetView.x = 
			p->my_car.maxWidthView;

	if(p->my_car.offsetView.y > 
			p->my_car.maxHeightView)
		p->my_car.offsetView.y = 
			p->my_car.maxHeightView;
}

void checkMapOffset(struct mapData *mapInfos)
{
	if(mapInfos->mapOffset.x < 0)
		mapInfos->mapOffset.x = 0;

	if(mapInfos->mapOffset.y < 0)
		mapInfos->mapOffset.y = 0;

	if(mapInfos->mapOffset.x > mapInfos->maxWidth)
		mapInfos->mapOffset.x = mapInfos->maxWidth;
		
	if(mapInfos->mapOffset.y > mapInfos->maxHeight)
		mapInfos->mapOffset.y = mapInfos->maxHeight;
		
}

void initMapOffset(struct mapData *mapInfos)
{
	struct cars *p;

	int w, h;

	p = mapInfos->p;
	/*
	mapInfos->mapOffset.x = 0;
	mapInfos->mapOffset.y = 0;

	mapInfos->mapOffset.w = mapInfos->screen->w;
	mapInfos->mapOffset.h = mapInfos->screen->h;

	w = mapInfos->mpBg->w - mapInfos->screen->w;

	if(w < 0)
		mapInfos->maxWidth = 0;
	else
		mapInfos->maxWidth = w;

	h = mapInfos->mpBg->h - mapInfos->screen->h;

	if(h < 0)
		mapInfos->maxHeight = 0;
	else
		mapInfos->maxHeight = h;
	*/
	while(p != NULL)
	{
		if(p -> my_car.computer == 0 || mapInfos->n_players == 1)
		{
			/* Find max offset for width and height */
			w = mapInfos->mpBg->w - p->my_car.destView.w;

			if(w < 0)
				p->my_car.maxWidthView = 0;
			else
				p->my_car.maxWidthView = w;

			h = mapInfos->mpBg->h - p->my_car.destView.h;

			if(h < 0)
				p->my_car.maxHeightView = 0;
			else
				p->my_car.maxHeightView = h;

			/* Set the offset to the start position */

			if(p->my_car.car_x > (p->my_car.destView.w/2) )
			{
				p->my_car.offsetView.x = 
					p->my_car.car_x - 
					( p->my_car.destView.w/2);

				checkViewOffset(p);
			}

			if(p->my_car.car_y > (p->my_car.destView.h/2) )
			{
				p->my_car.offsetView.y = 
					p->my_car.car_y - 
					( p->my_car.destView.h/2);

				checkViewOffset(p);
			}
			/* old code */
			/*
			if(p->my_car.car_x > mapInfos->screen->w/2)
			{
				
				mapInfos->mapOffset.x = p->my_car.car_x - 
					mapInfos->screen->w/2;
				
				checkMapOffset(mapInfos);
			}
			
			if(p->my_car.car_y > mapInfos->screen->h/2)
			{	
				mapInfos->mapOffset.y = p->my_car.car_y - 
					mapInfos->screen->h/2;

				checkMapOffset(mapInfos);
			}
			*/
			
		}
		
		p = p->next;
	}
	/*
	if(CODEDEBUG)
	{
		printf("Map Offset has been set to: %d %d \n", 
				mapInfos->mapOffset.x,
				mapInfos->mapOffset.y);
		printf("Maximum map offset is: %d %d\n",
				mapInfos->maxWidth,
				mapInfos->maxHeight);
	}*/
	
}

struct mapData * loadMap(
		SDL_Surface *screen, 
		TTF_Font *font,
		SDL_Joystick *joystick1, 
		char *map_name,
		int type)
{
	/*char car[10]="what";
	int pNumb = 2;
	int r = -1;*/
	int pNumb; 
	
	SDL_Rect dest;
	struct mapData *mapInfos = NULL;
	
	fillSurfaceWithBlack(screen);
	
	dest = print_hud_text(screen, "Loading...", 0, 0, font, 1);
	
	SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

	/* Loading map data : 
	 * maps/info/name.txt; 
	 * maps/name.png or maps/custom/name; 
	 * maps/name_checkp.png or maps/customcp/name_checkp
	 * optional : maps/name_road.png & maps/name_decoration.png */

	mapInfos = readMapTxt(map_name, type);
	
	if(mapInfos != NULL)
	{
		mapInfos->screen = screen;

		mapInfos->maxTours = MAX_LAPS;

		mapInfos->playersFinished = 0;

		mapInfos->timeElapsed = 0;

		mapInfos->joystick1 = joystick1;

		mapInfos->font = font;

		/*mapInfos->mpBg = NULL;
		mapInfos->mpCp = NULL;
		mapInfos->mpRoad = NULL;
		mapInfos->mpDecoration = NULL;*/

		checkCarData(mapInfos);
		
		pNumb = init_car_list(screen, mapInfos->p );
                mapInfos->n_players = pNumb;

		
		if( pNumb < 1 )
		{
			fprintf(stderr, "Unable to set up cars\n");
		}
		else
		{
			/*init_load_game_data(screen, map_name, &mpBg, &mpCp);*/
			if( 0 != init_load_game_data(
						screen, 
						map_name, 
						mapInfos, 
						type) )
			{
				fprintf(stderr, "Unable to load map images\n");

				clearMapData(&mapInfos);
			}
			else
			{
				
				initMapOffset(mapInfos);
				
				loadMoreMapImg(mapInfos);
				
				hudInitImg(mapInfos);
			}

			

			/*loadAllMapImg(screen, mapInfos, map_name);*/
		}
		
		if(mapInfos != NULL)
		{
			if(mapInfos->mpBg == NULL || mapInfos->mpCp == NULL)
			{
	
				clearMapData(&mapInfos);
			}
		}
	}
	

	/*launch_a_game(screen,map_name,&mpBg,&mpCp,&pNumb,joystick1, font,car);*/

	return(mapInfos);
}

void loadErrorReport(SDL_Surface *screen,
		TTF_Font *font,
		SDL_Joystick *joystick1)
{
}

void loadIntroduction(
		SDL_Surface *screen,
		TTF_Font *font,
		SDL_Joystick *joystick1)
{
	SDL_Surface *img;

	img  = surface_from_img(SPLASH_IMG);

	SDL_BlitSurface(img, NULL, screen, NULL);

	/*textOnScreen(screen,
			"Press any key to start",
			font,
			screen->w/2,
			(screen->h*0.05),
			2,
			1);*/

	SDL_Flip(screen);

	keyPause(2000);

	menuImgClear(&img);
}

void loadGameMenu(
	SDL_Surface *screen, 
	TTF_Font *font, 
	SDL_Joystick *joystick1)
{
	/* ---------------------------------------------- */
	/* default map name */
	
	SDL_Surface 
		*menuImgBack = NULL,
		*menuImgSelector = NULL;
	
	char mapName[256] = "";

	int status = 0, nb = 0;

	struct mapData *mapInfos = NULL;
	/* ---------------------------------------------- */

	loadMenuImg(&menuImgBack, &menuImgSelector);

	while(menuImgBack != NULL && menuImgSelector != NULL)
	{
	

		/*if(status >= 0)*/
		{

			status = menuShow_Principal(
					screen, 
					font, 
					joystick1, 
					menuImgBack, 
					menuImgSelector);

			switch(status)
			{
				case MENU_PRINCIPAL_SINGLE:

					nb = 0;
					
					/*menuShowFromData(
							screen, 
							font, 
							joystick1, 
							menuImgBack,
						       	menuImgSelector, 
							"menu/single.txt");*/

					do
					{		
						nb = menuShow_Single(
							screen, 
							font, 
							joystick1, 
							menuImgBack, 
							menuImgSelector, 
							mapName);
						
						if(nb > -1)
						{
							/* TO BE DELETED */
							/*nb = -1;*/
							menuImgClear(&menuImgBack);
							
							menuImgClear(&menuImgSelector);
							
							mapInfos = loadMap(
									screen, 
									font, 
									joystick1, 
									mapName,
									MAP_TYPE_STATIC);
							/*
							mapInfos = loadMap(screen, font, joystick1,
									"map1",
									MAP_TYPE_CUSTOM);
							*/
							if( mapInfos != NULL)
							{
								/* TO-DO start race */
								
								
								startRace(joystick1, 
									font, 
									mapInfos);
								
								clearMapData(&mapInfos);
							}
							else
							{
								loadErrorReport(
									screen, 
									font, 
									joystick1);
							}
	
							loadMenuImg(
								&menuImgBack, 
								&menuImgSelector);
							
						}
					} while(nb > -1);
					
				break;

				case MENU_PRINCIPAL_MULTI:
					break;

				default :
					menuImgClear(&menuImgBack);
					
					menuImgClear(&menuImgSelector);

				
					
			}
			

		}
		
		/*	
		startGameMenu_Principal(screen, font, joystick1,
				menuImgBack, menuImgSelector);*/

	}
		
	
	

	menuImgClear(&menuImgBack);
	
	menuImgClear(&menuImgSelector);

}


SDL_Joystick *loadInput()
{
	int num_joysticks, iBcl;
	SDL_Joystick *joystick = NULL;

	num_joysticks = SDL_NumJoysticks();
	
	if(num_joysticks > 0)
	{
		SDL_JoystickEventState(SDL_ENABLE);

		joystick = SDL_JoystickOpen(iBcl);
		
		
		if(CODEDEBUG)
		{
			printf(" - loadInput() : Here is the list of ");
			printf("Joysticks found. Only the first will be used.\n");
		
		}
	}
	
	if(CODEDEBUG)
	{
		for( iBcl = 0 ; iBcl < num_joysticks; iBcl++ ) 
		{
			
			printf("%d %s\n", (iBcl+1), SDL_JoystickName(iBcl) );
			
		}
	}
	
	return(joystick);
}

TTF_Font* loadFont(char *path, int height)
{
	TTF_Font* font = NULL;
	
	if(path == NULL)
		return(NULL);
	
	font = TTF_OpenFont(path, height);
	
	if (font == NULL)
	{
		fprintf(stderr,
			"Unable to load font: %s %s \n", 
			path, 
			TTF_GetError());
	}
	
	return font;
}


