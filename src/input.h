#ifndef __INPUT_H__
#define __INPUT_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

int getKeyboard(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, char *txt, int size, char *output);


#endif
