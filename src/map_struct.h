#ifndef __MAP_STRUCT_H__
#define __MAP_STRUCT_H__

#include "struct_car.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

struct mapData {
	char *longName;
	int totalCheckpoints;
	int bestLapTime;
	int goldenLapTime;
	int maxPlayers;
	int n_players;
	int playersFinished;
	int maxTours;
	Uint32 timeElapsed;
	SDL_Surface *mpBg;
	SDL_Surface *mpCp;
	SDL_Surface *mpRoad;
	SDL_Surface *mpDecoration;
	SDL_Surface *mpCollision;
	sge_cdata *cmap;

	SDL_Surface *hudSpeed;
	SDL_Surface *hudLaps;

	TTF_Font *font;
	SDL_Joystick *joystick1;

	SDL_Rect mapOffset;
	int maxWidth;
	int maxHeight;
	struct cars *p;
        struct ai_data* drivingdata;
	
	SDL_Surface *screen;
};

#endif
