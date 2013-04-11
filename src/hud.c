#include <SDL/SDL.h>

#include "config.h"

#include "image.h"
#include "hud.h"

SDL_Surface* hudImgInit(char *path)
{
	SDL_Surface *img = NULL;
	
	img = surface_from_img(path);
	
	if(img != NULL)
	{
		SDL_SetColorKey(img, SDL_SRCCOLORKEY, 
			SDL_MapRGB(img->format, 0, 0, 255));
		
		SDL_SetAlpha(img, SDL_SRCALPHA, 190);

	}
	
	return(img);
}


void hudInitImg(struct mapData *r)
{	
	r->hudSpeed = hudImgInit(DATA_DIR"/images/hud/speed.png");

	r->hudLaps = hudImgInit(DATA_DIR"/images/hud/laps.png");


	
}
