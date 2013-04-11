#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "main.h"
#include "editor.h"
#include "custom.h"
#include "map_struct.h"



void initCustomMapData(
	SDL_Surface *screen,
	char *selected_map,
	struct mapData *mapInfos)
	/*
	SDL_Surface **map_background,
	SDL_Surface **map_checkpoints)*/
{
	mapInfos->mpBg = customLoadSurface(
			screen, 
			MAX_MAP_WIDTH*5, MAX_MAP_HEIGHT*5, 
			SDL_MapRGB(screen->format,
				0, 121, 16));
	
	if(mapInfos->mpBg != NULL)
	{
		customLoadPieces(mapInfos->mpBg, selected_map);
		
		mapInfos->mpCp = customLoadSurface(
				screen, 
				MAX_MAP_WIDTH*5,
				MAX_MAP_HEIGHT*5,
				SDL_MapRGB(screen->format,
					248, 248, 248));

		if(mapInfos->mpCp != NULL)
			customLoadCheckp(mapInfos->mpCp, selected_map);
		
	}
	
}

int initFont()
{
	/* Init TTF loader */
	if (TTF_Init() < 0) 
	{
		fprintf(stderr,
			"Unable to initialize SDL_ttf: %s \n", 
			TTF_GetError());
		return(-1);
	}
	
	return(0);
}


SDL_Surface* initScreen()
{
	SDL_Surface *scr = NULL;
	
	/* Set Video Mode */
	scr = SDL_SetVideoMode(
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		COLOR_DEPTH, 
		SDL_SWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF
		/*	|SDL_FULLSCREEN
			|SDL_DOUBLEBUF*/
		);
	
	if( scr == NULL ) 
	{
		fprintf(
			stderr, 
			"Couldn't set %dx%dx%d video mode: %s\n",
			SCREEN_WIDTH,
			SCREEN_HEIGHT, 
			COLOR_DEPTH, 
			SDL_GetError());
		
		return (NULL);
	}

	
	return(scr);

}
