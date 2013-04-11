#ifndef __CAR_MOVE_H__
#define __CAR_MOVE_H__

#define TURNANGLE 10

#include "SDL/SDL.h"
#include "struct_car.h" 

void car_turn(struct car_properties *car_player, 
		int millitime, int turn);

void move_advanced(
	SDL_Surface *background,
	struct car_properties *car_player,
	int millitime);

void moveCar(
	struct car_properties *car_player, 
	float x, 
	float y,
	SDL_Surface *background);

void car_move_fw(struct car_properties *car_player, 
		int millitime);
/*
void ralentir(struct car_properties *car_player, int millitime);
*/
void car_move_bk(struct car_properties *car_player, int millitime);
/*
void car_move_lf(struct car_properties *car_player);

void car_move_rg(struct car_properties *car_player);
*/
void forward_noaccel(struct car_properties *car_player, int millitime);
/*
int grass(struct car_properties *car_player);

void road(struct car_properties *car_player);
*/
#endif
