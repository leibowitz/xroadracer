#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "config.h"

#include "main.h"
#include "pixel.h"
#include "debug.h"
#include "struct_car.h"
#include "map_struct.h"

#define IMGDIR DATA_DIR"/images/\0"
#define LIGHTSDIR DATA_DIR"/images/lights/"

void copySurfaceToScreen(
	SDL_Surface *screen,
	SDL_Surface *img,
	int x, int y, int w, int h)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;

	rect.w = w;
	rect.h = h;

	SDL_BlitSurface(img, &rect, screen, &rect);
	
	/*SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);*/
}

void copy_car_toscreen(
	SDL_Surface * pcar_buffer,
	SDL_Surface *pscreen, 
	SDL_Rect *pdest,
	struct cars *p)
{
	int xBcl, yBcl, 
		decal_x = 0, decal_y = 0, 
		max_x , max_y;
	Uint8 r,g,b;
	Uint32 color;
		
	
	if(pcar_buffer == NULL || pscreen == NULL || pdest == NULL)
	{
		fprintf(stderr, "Unable to draw the cars on the screen\n");
		if(CODEDEBUG)
			fprintf(stderr, "The error comes from outside of copy_car_toscreen()\n");
		return;
	}

	
	if(pdest->x >= p->my_car.destView.w || 
			pdest->y >= p->my_car.destView.h )
	{
		if(CODEDEBUG)
			fprintf(stderr, "The car is outside the borders\n");
		return;
	}

	if(pdest->x < -pcar_buffer->w || 
			pdest->y < -pcar_buffer->h)

	{
		if(CODEDEBUG)
			printf("The car is outside the screen\n");
		return;
	}
		
	decal_x = pdest->x + p->my_car.destView.x;
	decal_y = pdest->y + p->my_car.destView.y;
	
	max_x = decal_x + pcar_buffer->w;
	max_y = decal_y + pcar_buffer->h;

	if(decal_x < p->my_car.destView.x)
		decal_x = p->my_car.destView.x;
	else
		if(max_x > p->my_car.destView.x+p->my_car.destView.w)
			max_x = p->my_car.destView.x+p->my_car.destView.w;

	if(decal_y < p->my_car.destView.y)
		decal_y = p->my_car.destView.y;
	else
		if(max_y > p->my_car.destView.y+p->my_car.destView.h)
			max_y = p->my_car.destView.y+p->my_car.destView.h;
	/*
	if(decal_y < 0)
		decal_y = 0;
	else
		if(max_y > pscreen->h)
			max_y = pscreen->h;
	*/

	if ( SDL_MUSTLOCK(pscreen) ) 
	{
		if ( SDL_LockSurface(pscreen) < 0 ) 
		{
		        fprintf(stderr, "Can't lock screen: %s\n", 
		        SDL_GetError());
		        return;
		}
	}

	if(CODEDEBUG)
	printf(" - copy_car_to_screen() : starting to scan the car_buffer img\n");
		
	for(xBcl = decal_x; xBcl < max_x; xBcl++)
	{
		for(yBcl = decal_y; yBcl < max_y; yBcl++)
		{
			color = getpixel(pcar_buffer, 
					xBcl-pdest->x-p->my_car.destView.x, 
					yBcl-pdest->y-p->my_car.destView.y);
			
			SDL_GetRGB(color, 
				pcar_buffer->format, 
				&r,&g,&b);

			if(!(r < 10 && g-b < 15 && b-g < 15 && b > 30))
			{
				putpixel(pscreen,
					xBcl,
					yBcl,
					color);
			}
		}	
	}
	
	if ( SDL_MUSTLOCK(pscreen) )
		SDL_UnlockSurface(pscreen);
	
	if(CODEDEBUG)
		printf("car copied on screen\n");
} 

SDL_Surface * Load_img_from(char *file)
{
	SDL_Surface * image;
	/*printf("tac-1\n");*/
	/*printf("-%s-\n",file);*/
	image = IMG_Load(file);
	/*printf("tac-2\n");*/
	if (image == NULL)
	{
		/*fprintf(stderr,
			"%s\n",
			file, IMG_GetError());
		*/
		return NULL;
	}
	return image;
}

void show_img_xy(
	SDL_Surface *screen, 
	char img[], 
	int x, 
	int y)
{
	SDL_Surface 
		*image = NULL,
		*format_img = NULL;
	
	SDL_Rect r = {0, 0, 1, 1};
	
	image = Load_img_from(img);

	if(image == NULL)
	{
		fprintf(stderr, "Unable to load img: %s\n",img);
		return;
	}

	format_img = SDL_DisplayFormat(image);

	if(format_img != NULL)
	{
		r.x = x;
		r.y = y;

		r.w = format_img->w;
		r.h = format_img->h;

		SDL_BlitSurface(format_img, 
				NULL,
				screen,
			        &r);

		/*SDL_Flip(screen);*/
		SDL_UpdateRect(screen, r.x, r.y, r.w, r.h);

		SDL_FreeSurface(format_img);

		format_img = NULL;
	}

	SDL_FreeSurface(image);

	image = NULL;
		
}

SDL_Surface * show_splash(
	SDL_Surface *screen, 
	char img[])
{
	SDL_Surface 
		*image = NULL,
		*loaded_logo = NULL;
	
	char *sdl_logo_png = img;
	/*char sdl_logo_png[] = "images/sdl_logo_640x480.png\0";*/
	
	/* Loading SDL logo */
	/*printf("pouet -  %s\n",img);*/
	image = Load_img_from(sdl_logo_png);
	/*printf("prout\n");*/
	
	if(image != NULL)
	{
		loaded_logo = SDL_DisplayFormat(image);

		/* clean up memory */
		SDL_FreeSurface(image);
		image = NULL;
		
		if(loaded_logo != NULL)
		{
			SDL_BlitSurface(loaded_logo, NULL,
			screen, NULL);
			
			SDL_Flip(screen);
			
			/* Flip is the same as this */
			/*fade (screen, loaded_logo, 2, 0) ;*/
			/* Print the logo during 1sec */
			/*SDL_Delay(1000);*/
			/*fade (screen, loaded_logo, 2, 1) ;*/
			/* then clear the surface */
		}
		
	}
	
	return(loaded_logo);
}

SDL_Surface* surface_from_img(char *img_name)
{
	SDL_Surface *surface, *rSurf = NULL;

	surface = Load_img_from(img_name);

	if(surface != NULL)
	{
		rSurf = SDL_DisplayFormat(surface);

		SDL_FreeSurface(surface);

		surface = NULL;
	}
	
	return(rSurf);
}


void show_start_light_img_on_the_middle(
	SDL_Surface *screen,
	SDL_Surface *background,
	int type)
{
	char img[256] = IMGDIR;
	int x, y;
	int w = 120, h = 40;
	
	switch(type)
	{
		case 3:
			strcat(img, "start_light_three");
			break;
		case 2:
			strcat(img, "start_light_two");
			break;
		case 1:
			strcat(img, "start_light_one");
			break;
			
		case 0:
			strcat(img, "start_light_go");
			break;
	}

	strcat(img, ".png");

	/*printf("image %s\n",img);*/
	
	x = (SCREEN_WIDTH/2) - (w/2) ;
	y = 0; /*(SCREEN_HEIGHT/2) - (h/2) ;*/

	
	copySurfaceToScreen(screen, background, x, y, w, h);

	show_img_xy(screen, img, x, y);
}

	

void showStartLight(SDL_Surface *screen,
		int whatLight, 
		SDL_Surface **s)
{
	
	char img[256] = LIGHTSDIR;

	if(*s == NULL)
	{

		switch(whatLight)
		{
			case 3:
				strcat(img, "start_light_three");
				break;
			case 2:
				strcat(img, "start_light_two");
				break;
			case 1:
				strcat(img, "start_light_one");
				break;
				
			case 0:
				strcat(img, "start_light_go");
				break;
		}

		strcat(img, ".png");

		/*printf("image %s\n",img);*/
		*s = surface_from_img(img);

		if(*s != NULL)
		{
			SDL_SetAlpha(*s, SDL_SRCALPHA, 240);
		}


	}

}

void blitStartLight(struct mapData *mapInfos, SDL_Surface *s)
{
	struct cars *p = mapInfos->p;
	
	SDL_Rect dest;

	if(s != NULL)
	{
		while(p != NULL)
		{
	
			if(p->my_car.computer == 0)
			{
		
				dest.y = p->my_car.destView.y;
			
				dest.x = p->my_car.destView.x+
						((p->my_car.destView.w - s->w)/2);

				SDL_BlitSurface (s, NULL, mapInfos->screen, &dest);
			}
		
	
			p = p->next;
		}
	}
}
