#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "text.h"

void updateText(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, char *txt, 
	char *output)
{

	SDL_BlitSurface (background, NULL, screen, NULL);
	
	textOnScreen(screen, output, font,
			screen->w/2,
			screen->h/2,
			1,
			5);
	
	textOnScreen(screen, txt, font, 
			screen->w/2,
			screen->h/2+20,
			1,
			5);

	SDL_Flip(screen);
}

int getKeyboard(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, char *txt, int size, char *output)
{
	SDL_Event event;

	int pos = 0,
	    loop = 1;

	while(loop)
	{

		SDL_EnableUNICODE(1);

		while(SDL_PollEvent(&event) )
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					/*printf("__________\n");
					printf("%s\n", SDL_GetKeyName(event.key.keysym.sym) );
					printf("%04X\n", event.key.keysym.unicode);*/

					if(event.key.keysym.sym >= SDLK_a && 
						event.key.keysym.sym <= SDLK_z)
					{
						if(pos < size)
						{
							*(txt+pos) = event.key.keysym.sym;
							
							pos++;
						}
						
						/*updateText(screen, font, txt);*/

					}
					else if(event.key.keysym.unicode >= 0x30 && 
						event.key.keysym.unicode <= 0x39)
					{
						if(pos < size)
						{
							/*if(event.key.keysym.scancode == 0x0B)
								*(txt+pos) = '0';
							else*/
							*(txt+pos) = event.key.keysym.unicode;

							pos++;
						}
					}
					else
					{
						switch((int)event.key.keysym.sym)
						{
							/*
							if(event.key.keysym.mod & KMOD_SHIFT && pos < size)
							{
								case SDLK_AMPERSAND:
									*(txt+pos) = '1';
									pos++;
									break;
							
								case SDLK_WORLD_73:
									*(txt+pos) = '2';
									pos++;
									break;

								case SDLK_QUOTEDBL:
									*(txt+pos) = '3';
									pos++;
									break;
									
								case SDLK_QUOTE:
									*(txt+pos) = '4';
									pos++;
									break;
									
								case SDLK_LEFTBRACKET:
									*(txt+pos) = '5';
									pos++;
									break;
									
								case SDLK_WORLD_22:
									*(txt+pos) = '6';
									pos++;
									break;
									
								case SDLK_WORLD_72:
									*(txt+pos) = '7';
									pos++;
									break;
									
								case SDLK_EXCLAIM:
									*(txt+pos) = '8';
									pos++;
									break;
									
								case SDLK_WORLD_71:
									*(txt+pos) = '9';
									pos++;
									break;
									
								case SDLK_WORLD_64:
									*(txt+pos) = '0';
									pos++;
									break;
									
							}
							*/
							case SDLK_BACKSPACE:
								if(pos > 0)
									pos--;
								*(txt+pos)='\0';
								break;

							case SDLK_DELETE:
								break;

							/*case SDLK_SPACE:
								*(txt+pos) = ' ';
								pos++;
								
								break;*/
							case SDLK_ESCAPE:
								pos = 0;
								loop = 0;
								break;

							case SDLK_RETURN:
							case SDLK_KP_ENTER:
								if(pos > 0)
									loop = 0;
								break;
						}
						/*if(event.key.keysym.sym == SDLK_BACKSPACE)
						{
						}
						else
						if(event.key.keysym.sym == SDLK_DELETE)
						{
						}
						else
						if(event.key.keysym.sym)
							;*/
					}
					break;
			}
		}
		
		updateText(background, screen, font, txt, output);
	}

	return(pos);
}


