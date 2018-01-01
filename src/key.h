#ifndef __KEY_H__
#define __KEY_H__

#define KEY_MOVEUP 0
#define KEY_MOVERIGHT 1
#define KEY_MOVEDOWN 2
#define KEY_MOVELEFT 3

#include "SDL/SDL.h"
#include "struct_car.h"
#include "map_struct.h"

void PrintModifiers( SDLMod mod );

void PrintKeyInfo( SDL_KeyboardEvent *key );

void keyPause(Uint32 maxTime);

void key_handling_press(
	SDL_Event *p_event, 
	int **p_key_fct_tab);

void key_read_config(int *p_key_fct_tab[], int *p_key_fct_sdlcode[]);

void keyCheckCommand(struct car_properties *c, int frameTime, int r);

int keyGetEvents( 
	int game_started, 
	int *pause, 
	int *debug_ia,
	int *hud,
	struct mapData *mapInfos);
#endif
