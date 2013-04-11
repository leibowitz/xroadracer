
#ifndef __LOAD_H__
#define __LOAD_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "map_struct.h"

void loadMenuImg(SDL_Surface **back, SDL_Surface **selector);

void menuImgClear(SDL_Surface ** s);

void clearMapData(struct mapData **mapInfos);

/*void loadAllMapImg(SDL_Surface *screen, struct mapData *mapInfos, char *mapName);*/

void checkCarData(struct mapData *mapInfos);

void loadMoreMapImg(struct mapData *mapInfos);

void checkMapOffset(struct mapData *mapInfos);

void initMapOffset(struct mapData *mapInfos);

struct mapData * loadMap(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1, char *map_name, int type);

void loadErrorReport(SDL_Surface *screen,
		TTF_Font *font,
		SDL_Joystick *joystick1);

void loadIntroduction(
		SDL_Surface *screen,
		TTF_Font *font,
		SDL_Joystick *joystick1);

void loadGameMenu(
	SDL_Surface *screen, 
	TTF_Font *font, 
	SDL_Joystick *joystick1);

SDL_Joystick *loadInput();

TTF_Font* loadFont(char *path, int height);

#endif
