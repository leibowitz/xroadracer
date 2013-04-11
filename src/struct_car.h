#ifndef __STRUCT_CAR_H__
#define __STRUCT_CAR_H__

#include <SDL/sge.h>
#include <SDL/SDL.h>

#include "define.h"

#define KEYS_DEFINED 4

struct playerKeys{
	int keys[KEYS_DEFINED];
};

struct car_properties 
{
	char car_img[256];
	char player_name[100];
	SDL_Surface *car;
	SDL_Surface *my_car_buffer;
	sge_cdata *cmap_surface;
	double SPEED_MAX;
	double ADD_SPEED;
	double car_x; 
	double car_y;
	double last_x; 
	double last_y;
	double velx;
	double vely;
	int xvel; 
	int yvel;
	int maxD;
	float rotation;
	float img_rotation;
	double speed;

	int playerNumber;

	int time_penality;
	int is_grass;
	int debugAi;
	int finished;
	int computer;
	Uint32 lap_time[MAX_LAPS];
	int total_time;
	int position;
	int last_elapsed;
	Uint32 startTime;
	int best_lap;
	int num_lap;
	int collision;
	int grass_detection;
	int rotation_side;
	int turning;

	int missed;
	Uint8 last_checkpoint_color;
	int last_checkpoint_x;
	int last_checkpoint_y;
	float last_checkpoint_rotation;
	Uint32 cpcolor;
	Uint32 bgcolor;
	int car_width;
	int car_height;
	struct playerKeys k;
	SDL_Rect destView;
	SDL_Rect offsetView;
	int maxWidthView;
	int maxHeightView;
	/*int viewWidth;
	int viewHeight;
	int viewX;
	int viewY;*/
};

struct cars
{
	struct car_properties my_car;
	struct cars *next;
	struct cars *prev;
};

#endif
