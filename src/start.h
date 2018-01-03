#ifndef __START_H__
#define __START_H__

#define MAP_STATIC_COLLISION DATA_DIR"/maps/static/%s_collision.png"
#define MAP_STATIC_ROAD DATA_DIR"/maps/static/%s_road.png"

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "map_struct.h"

int init_load_game_data(
	SDL_Surface *screen, 
	char *selected_map,
	struct mapData *mapInfos,
	/*SDL_Surface **map_background, 
	SDL_Surface **map_checkpoints,*/
	int type);

void startRace(SDL_Joystick *joystick1, TTF_Font *font, struct mapData *mapInfos);

/*void init_load_cars(
	SDL_Surface *screen,
	int *p_players_number);

void startCustomMap(
	SDL_Surface *screen, 
	char *selected_map_name,
	SDL_Surface **map_background,
	SDL_Surface **map_checkpoints,
	int *p_players_number,
	SDL_Joystick *the_joy,
	TTF_Font* font,
	char *selected_car_name);

void launch_a_game(
	SDL_Surface *screen, 
	char *selected_map_name,
	SDL_Surface **map_background,
	SDL_Surface **map_checkpoints,
	int *p_players_number,
	SDL_Joystick *the_joy,
	TTF_Font* font,
	char *selected_car_name);


*/
#endif
