#include <math.h>
#include <SDL/SDL.h>

#include "struct_car.h"
#include "car_move.h"

#define FRAMETIME_RATIO 17

/* Acceleration */
const double ADD_SPEED=0.007;
const double REM_SPEED=0.005833;
const double ADD_SPEED_ROAD=0.007;
const double ADD_SPEED_GRASS = 0.005;
const double MIN_SPEED_IA=0.075;

double vitesse_add=0.007;

/* Deceleration */
const double vitesse_del=0.05;

/* On the grass */
const double vitesse_del_ongrass=0.0025;

const double MAX_SPEED_ROAD = 7.0; /*4.90;*/ /* 3.5*/
const double MAX_SPEED_GRASS = 0.50;

void car_turn(struct car_properties *car_player, int millitime, int turn)
{
	car_player->rotation += turn * (millitime/(float)8);

	while(car_player->rotation > 360)
		car_player->rotation -= 360;

	if(car_player->rotation < 0)
		car_player->rotation += 360;
}

void move_advanced(
	SDL_Surface *background,
	struct car_properties *car_player,
	int millitime)
{
	/*const int VITESSE_MAX = 500;*/
	
	
	double sin_a, cos_a;
	/*float calcul_angle = rotation;*/
	float x, y;
	
	float piradian;

	piradian = (car_player->rotation)*M_PI/180.0;
	
	sin_a = sin(piradian);
	cos_a = cos(piradian);

	x = cos_a *(car_player->speed)/(float)FRAMETIME_RATIO;

	y = sin_a *(car_player->speed)/(float)FRAMETIME_RATIO;
	
	moveCar(car_player, x, y, background);
	
	return;
}

void moveCar(struct car_properties *car_player, 
		float velx, 
		float vely,
		SDL_Surface *background)
{
	/*if(fabs(velx) < 0.1 && 
		fabs(vely) < 0.1)
	{
		car_player->speed = 0.0;
	}*/

	car_player->last_x = car_player->car_x;
	car_player->last_y = car_player->car_y;

	/*if(millitime > 0)*/
	{
	car_player->velx = velx;
	car_player->vely = vely;

	}/*
	else
	{
		car_player->velx = 0;
		car_player->vely = 0;
	}
	
	car_player->car_x += velx;
	car_player->car_y += vely;
	*/
	return;
}
/* Faire avancer la voiture */
void car_move_fw(struct car_properties *car_player, int millitime)
{
	/*if(car_player == NULL)
		return;*/
	if (car_player->speed < car_player->SPEED_MAX)
		car_player->speed += vitesse_add * millitime/FRAMETIME_RATIO;

}

/* Fonction de ralentissement appellée lorsque la voiture dépasse 
les limites de la piste */
void car_move_bk(struct car_properties *car_player, int millitime)
{
	car_player->speed -= vitesse_del * millitime/FRAMETIME_RATIO;
	
	if(car_player->speed < vitesse_add)
		car_player->speed = 0.0;
}
/* Faire ralentir la voiture */
/*void car_move_bk(struct car_properties *car_player, int millitime)
{
	ralentir(car_player, millitime);
}*/
/* Pivoter la voiture vers la gauche */
/*
void car_move_lf(struct car_properties *car_player)
{
	car_player->xvel = -1;
}
*/
/* Pivoter la voiture vers la droite */
/*
void car_move_rg(struct car_properties *car_player)
{
	car_player->xvel = 1;
	

}
*/

/* faire avancer la voiture par inertie ; avec ralentissement */
void forward_noaccel(struct car_properties *car_player, int millitime)
{
	car_player->speed -= REM_SPEED*millitime/(FRAMETIME_RATIO);
	
	if(car_player->speed < vitesse_add)
		car_player->speed = 0.0;
	
}
/*
int grass(struct car_properties *car_player)
{
	car_player -> SPEED_MAX = MAX_SPEED_GRASS;
	car_player -> ADD_SPEED = ADD_SPEED_GRASS;
	
	if(car_player -> is_grass == 1)
		return(0);
	else
		car_player -> is_grass = 1;
	
	return(car_player -> is_grass);
	
	return(car_player -> is_grass);
}
void road(struct car_properties *car_player)
{
	car_player -> SPEED_MAX = MAX_SPEED_ROAD;
	car_player -> ADD_SPEED = ADD_SPEED_ROAD;
	
	if(car_player-> computer)
		car_player -> SPEED_MAX += 1.4;
	
	car_player -> is_grass = 0;
}
*/
