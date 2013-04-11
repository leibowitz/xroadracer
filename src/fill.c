#include <SDL/SDL.h>
#include <stdio.h>


void fillSurfaceWithColor(SDL_Surface *surf, Uint32 color)
{
	SDL_Rect dstrect;
	
	dstrect.x = 0; 
	dstrect.y = 0;
	dstrect.w = surf->w; 
	dstrect.h = surf->h;
	
	SDL_FillRect(surf, &dstrect, color);
	
	return;
}

void fillSurfaceWithBlack(SDL_Surface *surf)
{
	Uint32 color;
	
	/* Set Background color */
	color = SDL_MapRGB(surf->format, 0x00, 0x00, 0x00);

	/* fill black color on the screen */
	fillSurfaceWithColor(surf, color);
	
}
