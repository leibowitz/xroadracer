#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>

#include "config.h"

#include "image.h"

#define MAP_IMGBG_FORMAT DATA_DIR"/maps/static/%s.png"
#define MAP_IMGCP_FORMAT DATA_DIR"/maps/static/%s_checkp.png"

SDL_Surface* load_map_image(
	char *map_name,
	char *format)
{
	char img[256] = "";
	SDL_Surface *s = NULL;

	if(strlen(map_name) + strlen(format) < 250)
	{
		sprintf(img, format, map_name);
	
		s = surface_from_img(img);
	}

	return(s);
}

SDL_Surface* load_background_map(
	SDL_Surface *screen, 
	char *map_name)
{
	SDL_Surface 
		*image, 
		*loaded_logo = NULL;
	
	char background_img[256] = "";
	
	sprintf(background_img, MAP_IMGBG_FORMAT, map_name);
	/*strcat(background_img,map_name);
	strcat(background_img,".png\0");*/
	
	/* Loading Background */
	image = Load_img_from(background_img);
	
	if(image != NULL)
	{
		loaded_logo = SDL_DisplayFormat(image);

		/* clean up memory */
		SDL_FreeSurface(image);
		image = NULL;
		
		if(loaded_logo != NULL)
		{
			/*
			printf("%s loaded %dx%d\n",
				background_img,
				loaded_logo->w,loaded_logo->h);
			*/
			SDL_BlitSurface(
				loaded_logo, NULL,
				screen, NULL);
			
			/* Flip is the same as this */
			/*SDL_UpdateRect(screen, 0, 0, 0 ,0);*/
			/*SDL_Flip(screen);*/

			/*get_color_from_surface(screen);*/
			/*fade (screen, loaded_logo, 2, 0) ;*/
			/* Print the logo during 1sec */
			/*SDL_Delay(1000);
			fade (screen, loaded_logo, 2, 1) ;*/
			/* then clear the surface */
		}
		
	}
	
	return(loaded_logo);

}

	
SDL_Surface* load_checkpoints(
	SDL_Surface *screen, 
	char *map_name)
{
	SDL_Surface *checkpoints = NULL,
		*image = NULL;
	
	char checkpoints_img[256] = "";
	
	
	sprintf(checkpoints_img, MAP_IMGCP_FORMAT, map_name);
	/*strcat(checkpoints_img,map_name);
	strcat(checkpoints_img,"_checkp.png\0");*/
	
	/* Loading CheckPoints */
	
	image = Load_img_from(checkpoints_img);
	
	if(image != NULL)
	{
		checkpoints = SDL_DisplayFormat(image);

		/* clean up memory */
		SDL_FreeSurface(image);
		image = NULL;
		
	}
	
	return(checkpoints);
}
