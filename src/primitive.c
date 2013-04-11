#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

#include "pixel.h"
#include "collision.h"


void drawPointPattern(
	int x, 
	int y, 
	Uint32 color, 
	SDL_Surface *sDest)
{
	int ibcl, jbcl;

	for(ibcl = x - 5; ibcl < (x+5); ibcl++)
	{
		for(jbcl = y -5; jbcl < (y+5); jbcl++)
		{
			putpixel(sDest, ibcl, jbcl, color);
		}
	}
	
}

void drawLineAB(
	float x1, 
	float y1, 
	float x2, 
	float y2, 
	Uint32 color, 
	SDL_Surface *sDest)
{
	float ang;

	int bcl = 0;
	
	ang = collisionGetAngle( x1, y1, x2, y2);

	if(SDL_MUSTLOCK(sDest))
		SDL_LockSurface(sDest);

	
	while(fabs(x1 - x2) > 10 || fabs(y1 - y2) > 10)
	{
		x1 += cos(ang/ARAD);
		y1 += sin(ang/ARAD);

		bcl++;
		
		drawPointPattern((int)x1, (int)y1, color, sDest);
	}
	
	if(SDL_MUSTLOCK(sDest))
		SDL_UnlockSurface(sDest);

	
}


