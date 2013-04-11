#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "config.h"

#include "main.h"
#include "editor.h"
#include "primitive.h"
#include "image.h"
#include "mask.h"

#define MAP_CUSTOMP_FORMAT DATA_DIR"/maps/custom/%s"
#define MAP_CUSTOMC_FORMAT DATA_DIR"/maps/custom/%s_checkp"
#define MAP_PIECES_FORMAT DATA_DIR"/construct/road_%03d.png"

SDL_Surface* customLoadSurface(
	SDL_Surface *screen, 
	int width,
	int height,
	Uint32 color)
{



	SDL_Surface *background, *bgTemp;

	background = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			width,
			height,
			COLOR_DEPTH,
			rmask,
			gmask,
			bmask,
			amask);

	if(background != NULL)
	{
		bgTemp = SDL_DisplayFormat(background);

		SDL_FreeSurface(background);

		if(bgTemp != NULL)
		{
			SDL_FillRect(bgTemp, NULL,color);

		}
		
		background = bgTemp;
	}
	return(background);
}

void customLoadCheckp(
	SDL_Surface *checkpoints, 
	char *map_name)
{
	FILE *file;
	int x1, y1, x2, y2, bcl = 0;
	char message[256];
	Uint32 color;
	
	sprintf(message,MAP_CUSTOMC_FORMAT, map_name);
	
	file = fopen(message,"r");

	if(file != NULL)
	{
		while(fscanf(file,"%d %d %d %d",&x1,&y1,&x2,&y2) == 4)
		{
			color = SDL_MapRGB(checkpoints->format,
					bcl*8, bcl*8, bcl*8);
		
			drawLineAB(
					(float)x1, 
					(float)y1, 
					(float)x2, 
					(float)y2, 
					color, 
					checkpoints);
			bcl++;
		}
		
		fclose(file);
	}
}

void customLoadPieces(
	SDL_Surface *background, 
	char *map_name)
{
	
	FILE *file;
	int x, y, n, maxPieces = CONSTRUCT_MAX_PIECES, bcl;/*,
	    width, height;*/
	char message[256];
	
	SDL_Rect position;

	Uint32 blue = SDL_MapRGB(background->format,
			0,0,255);
	
	SDL_Surface **availPieces = NULL;
	
	availPieces = (SDL_Surface**) 
		malloc(sizeof(SDL_Surface)*maxPieces);
	
	if(availPieces == NULL)
		return;
	
	for(bcl=0; bcl < maxPieces; bcl++)
	{
		sprintf(message, 
		MAP_PIECES_FORMAT,
		(bcl+1));

		availPieces[bcl] = surface_from_img(message);

		if(availPieces[bcl] != NULL)
		{
			SDL_SetColorKey(availPieces[bcl],
					SDL_SRCCOLORKEY,
					blue);
		}
	}

	sprintf(message,MAP_CUSTOMP_FORMAT, map_name);
	
	file = fopen(message,"r");

	if(file != NULL)
	{

		/*fscanf(file, "%d %d", &width, &height);

		initCustomBackground(screen, width, height, blue);*/

		while(fscanf(file,"%d %d %d",&x,&y,&n) == 3)
		{
			position.x = x*5;
			position.y = y*5;
			
			SDL_BlitSurface(availPieces[n], NULL,
				background, &position);
		
		
		}
		
		fclose(file);
	}

	for(bcl=0; bcl < maxPieces; bcl++)
	{
		free(availPieces[bcl]);
		
	}
	
	free(availPieces);
}

