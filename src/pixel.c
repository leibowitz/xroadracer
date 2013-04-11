#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

#include "pixel.h"

int debug_ia = 0;

/* Check pixel */
int check_pixel_xy(
	SDL_Surface * screen, 
	double dot_x, double dot_y,
	SDL_Surface *background,
	int xvel,
	SDL_Rect map_offset)
{
	Uint8 r,g,b;
	Uint32 color;
	int return_value = 0;
	/*
	Uint8 road_r = 111, road_g = 111, road_b = 111;
	Uint8 grass_r = 0, grass_g = 121, grass_b = 20;
	*/
	Uint8 road_r = 107, road_g = 109, road_b = 107;
	Uint8 grass_r = 0, grass_g = 121, grass_b = 16;
	
	/*SDL_Surface *rotated_car;
	double calcul_angle = (double)rotation,  
		cote_adj,cote_op,cote_hypo,
		hauteur,
		sin_a,cos_a*/;
	
	if(	((int)dot_x < background->w) && !((int)dot_x < 0) && 
		((int)dot_y < background->h) && !((int)dot_y < 0) )
	{
		/* Lock the screen for direct access to the pixels */
		if ( SDL_MUSTLOCK(screen) ) {
			if ( SDL_LockSurface(screen) < 0 ) 
			{
			        fprintf(stderr, "Can't lock screen: %s\n", 
			        SDL_GetError());
			        return(0);
			}
		}

	
		/* remember the color under the dot */
		color = getpixel(background, (int)dot_x, (int)dot_y);

		/* and the coordonate */
		/**back_x = dot_x;
		*back_y = dot_y;*/
		
		/* Get color form the exact pixel */
		SDL_GetRGB(color, screen->format, 
			&r,&g,&b);
		
		
		 if(
		  (r == 255 && r == g && g == b) || 
		  (r == 189 && g == 0 && b == 0) )
		 {
		 	return_value = 3;
		 }
		else
		/* draw black on green, and white everywhere else */
		if((r == grass_r && g == grass_g && b == grass_b ) 
		/*|| (r == 255 && r == g && g == b && xvel != 0) */)
		{
			/* draw the dot color */
			/*putpixel(screen, dot_x, dot_y, 
			SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));*/
			/*if(grass(vitesse) == 1)
			{
				*time += 50;
				printf("Grass Penalty, +0.5 sec\n");
			}*/
			return_value = 1;
		}
		/*else*/

	/*printf("x %lf  y %lf   r %d, g %d, b %d\n",dot_x, dot_y, r,g,b);*/
		
		if(r == road_r && g == road_g && b == road_b)
		{
			/*putpixel(screen, dot_x, dot_y, 
			SDL_MapRGB(screen->format, 0xff, 0xff, 0xff)
			);*/
			/*road(vitesse);*/
			/*printf("x %lf  y %lf road \n",dot_x, dot_y);*/
			return_value = 2;
		}
		/* paint */
		if(xvel > 0)
			color = SDL_MapRGB(screen->format, 
					0x00, 0x00, 0x00);
		else if(xvel < 0)
			color = SDL_MapRGB(screen->format, 
					0xff, 0xff, 0xff);
		else
			color = SDL_MapRGB(screen->format, 
					0x05, 0xff, 0xff);

		dot_x -= map_offset.x;
		dot_y -= map_offset.y;
			
		if(debug_ia == 1 && 
				(int)dot_x < screen->w && 
				(int)dot_y < screen->h && 
				(int)dot_x > 0 && 
				(int)dot_y > 0)
		{
			putpixel(screen, 
					dot_x, 
					dot_y, 
					color);
		}
		/* Unlock the screen surface */
		if ( SDL_MUSTLOCK(screen) )
			SDL_UnlockSurface(screen);
		
		if(debug_ia == 1 && 
				(int)dot_x < screen->w &&
				(int)dot_y < screen->h && 
				(int)dot_x > 0 && 
				(int)dot_y > 0)
		{
			SDL_UpdateRect(screen, 
				(int)dot_x, 
				(int)dot_y, 
				1, 
				1 );
		}
		
	}
	else
		return(1);
	
	return(return_value);
}


/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getpixel(
	SDL_Surface *surface, 
	int x, 
	int y)
{
	int bpp; 
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p;

	if(!(x >= 0 && y >= 0 && x < surface->w && y < surface->h))
		return(0);
	
	bpp = surface->format->BytesPerPixel;

	p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
		return *p;

		case 2:
		return *(Uint16 *)p;

		case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
		return *(Uint32 *)p;

		default:
		return 0;/* shouldn't happen, but avoids warnings */
	}
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(
	SDL_Surface 
	*surface, 
	int x, 
	int y, 
	Uint32 pixel)
{
	int bpp;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p;

	if(surface == NULL)
		return;

	if(!(x >= 0 && y >= 0 && x < surface->w && y < surface->h))
		return;

	bpp = surface->format->BytesPerPixel;


	p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
		*p = pixel;
		break;

		case 2:
		*(Uint16 *)p = pixel;
		break;

		case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
			} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

		case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}
