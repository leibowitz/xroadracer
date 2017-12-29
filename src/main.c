#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#include "config.h"

#include "main.h"
#include "editor.h"

#include "init.h"
#include "load.h"


#include "config.h"

#define FONT_PATH DATA_DIR"/fonts/VeraBd.ttf"

int main(int argc, char **argv)
{
	char *base_name = 0;
	
	/* ------------------------------------------- */
		
	if( 0 > SDL_Init ( SDL_INIT_VIDEO /*| 
			SDL_INIT_AUDIO | SDL_INIT_JOYSTICK*/) )
	{
		fprintf(stderr,
			"Could not initialize SDL: %s\n",
			SDL_GetError());
		
		exit(EXIT_FAILURE);
		
	}
	
	/*
	atexit(SQL_Quit);
	This could be set but I prefer to handle the SQL_QUIT myself
	*/

	/* Temporary name, before we know what we need to load */
	
	SDL_WM_SetCaption("Loading XRR", NULL);

        /* Detect the executable name from the first argument */
        
        if( ( base_name = strrchr(argv[0], '/') ) != 0 )
                ++base_name;
        else
                base_name = argv[0];
        
        loadGame(base_name);
	
	SDL_Quit();
	
	return(EXIT_SUCCESS);
}

void loadGame(char *base_name) {
	/* ------------------------------------------- */
	SDL_Surface *screen = NULL;
	
	SDL_Joystick *joystick1 = NULL;
	
	TTF_Font *font = NULL;
	
	char font_path[] = FONT_PATH;
	
	screen = initScreen();
	
	if(screen == NULL)
	{
                return;
        }

        if(initFont() != 0)
        {
                return;
        }

        font = loadFont(font_path, 12);
        
        if(font == NULL)
        {
                return;
        }
                
        /* Init the first joystick only */
        joystick1 = loadInput();
        
        /* Start menu and game loops */
        if(!strcmp(base_name, PACKAGE) ) 
        {
                SDL_ShowCursor(SDL_DISABLE);
                
                SDL_WM_SetCaption("X Road Racer - Press any key to start", NULL);

                loadIntroduction(screen, font, joystick1);

                SDL_WM_SetCaption("X Road Racer", NULL);
                
                loadGameMenu(screen, font, joystick1);
        }
        else
        {
                SDL_WM_SetCaption("Race Editor - for X Road Racer", 
                        NULL);
                
                startNewEditor(screen, font, joystick1);
        }


        if(joystick1 != NULL)
        {
                SDL_JoystickClose(joystick1);
        }
                
        TTF_CloseFont(font);
        
        font = NULL;
        
        TTF_Quit();
			
        /*
         * It appears that the screen surface will be 
         * freed by SDL_Quit() 

        SDL_FreeSurface(screen);
        screen = NULL;
        */
}

