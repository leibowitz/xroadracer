#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

#include "struct_car.h"
#include "collision.h"
#include "calcul.h"
#include "car_move.h"

#define MAXIMUM_EDGE_HIT_SPEED 2

void carReactionMove(
	struct cars *one_car, 
	double ang, 
	double dif, 
	int coef,
	int frameTime)
{
	int angular;
	
	if(fabs(ang) < dif)
		angular = 0;
	else
	{
		if(ang > dif)
			angular = coef;
		else
			angular = -1 * coef;
	}
	
	if(one_car != NULL)
	{
		
		one_car->my_car.rotation += angular * 2 * 
			one_car->my_car.speed;
		
		if( angular == 0 )
		{
			one_car->my_car.speed = 0;
		}/*
		else 
			if(one_car->my_car.speed > MAXIMUM_EDGE_HIT_SPEED)
				car_move_bk( &(one_car->my_car), frameTime );
		*/

	}
	

}

void carReaction(
	struct cars *one_car, 
	double cos, 
	double sin, 
	SDL_Surface *background,
	int frameTime)
{
	int coef = 2;

	double dif = 0.5;
	
	if(one_car != NULL)
	{
		if(sin < 0.0 && one_car->my_car.car_y < 20)
		{
			carReactionMove(one_car, cos, dif, coef,frameTime);
			/*if(cos < -dif)
				angular = -1 * coef;
			else
			{
				if(cos < dif)
					angular = 0;
				else
					angular = coef;
			}
			
			one_car->my_car.rotation += angular * 
				one_car->my_car.speed;

			if( angular == 0 )
			{
				one_car->my_car.speed = 0.1;
			}*/
		}
		else 
		if(sin > 0.0 && one_car->my_car.car_y > background->h -20)
		{
			carReactionMove(one_car, cos, dif, (-coef),frameTime);
			
			/*if(cos < -dif)
				angular = coef;
			else
			{
				if(cos < dif)
					angular = 0;
				else
					angular = -1 * coef;
			}

			one_car->my_car.rotation += angular * 
				one_car->my_car.speed;

			if( angular == 0 )
			{
				one_car->my_car.speed = 0.1;
			}*/
			
		}

		
		
		if( cos < 0.0 && one_car->my_car.car_x < 20 )
		{
			carReactionMove(one_car, sin, dif, (-coef),frameTime);
			/*if(sin < -dif)
				angular = coef;
			else
			{
				if(sin < dif)
					angular = 0;
				else
					angular = -1 *coef;
			}

			one_car->my_car.rotation += angular *
				one_car->my_car.speed;

			if( angular == 0 )
				one_car->my_car.speed = 0.1;*/
		}
		else
		if( cos > 0.0 && one_car->my_car.car_x > background->w -20 )
		{
			carReactionMove(one_car, sin, dif, coef,frameTime);
		}
	}
}

void carOnEdge(SDL_Surface *background, 
		struct cars *one_car, 
		int frameTime)
{
	int offsetX = 0, offsetY = 0;
	double cos, sin, rsin, rcos;
	float cl;
	
	while(one_car != NULL)
	{
		/*if((one_car->my_car.rotation >= 50 && 
				one_car->my_car.rotation < 130 ) ||  
		(one_car->my_car.rotation >=230 && 
				one_car->my_car.rotation < 310 )  )
		{
			if(one_car->my_car.car_y > background->h -20)
			one_car->my_car.car_y = background->h - 20;
			if(one_car->my_car.car_y < 20) 
			one_car->my_car.car_y = 20;
		}
		else*/
		{
			radius_rad(&cl, 
				one_car->my_car.rotation,
				&sin,
				&cos);
			/* - sinus - */
			if(one_car->my_car.rotation > 180)
				rsin = -sin;
			else
				rsin = sin;
			
			offsetY = 10 - round(10*rsin);
			
			/*printf("%.2lf %.1f\n",sin, rsin);*/
			
			if(one_car->my_car.car_y < 20 -offsetY)
			{
				carReaction(one_car, cos, sin,
						background, 
						frameTime);
				/*
				one_car->my_car.car_y -= 
					one_car->my_car.vely;

				one_car->my_car.vely = 0.0f;*/
				one_car->my_car.car_y = 20 -offsetY;

			}
			else
				if(one_car->my_car.car_y > 
					background->h -20 +offsetY)
				{
					carReaction(one_car, cos, sin,
							background,
							frameTime);
					/*one_car->my_car.car_y -= 
						one_car->my_car.vely;

					one_car->my_car.vely = 0.0f;*/
					one_car->my_car.car_y = 
					background->h -20 +offsetY;
				}
			
			/* - cosinus - */
			if(one_car->my_car.rotation < 270 && 
				one_car->my_car.rotation > 90)
				rcos = -cos;
			else
				rcos = cos;
			
			offsetX = 10 - round(10*rcos);

			if(one_car->my_car.car_x < 20 -offsetX)
			{
				carReaction(one_car, cos, sin,
						background,
						frameTime);
				/*one_car->my_car.car_x -= 
					one_car->my_car.velx;

				one_car->my_car.velx = 0.0f;*/
				one_car->my_car.car_x = 20 -offsetX;
			}
			else
				if(one_car->my_car.car_x > 
					background->w -20 +offsetX)
				{
					carReaction(one_car, cos, sin,
						background,
						frameTime);
					/*one_car->my_car.car_x -= 
						one_car->my_car.velx;

					one_car->my_car.velx = 0.0f;*/
					one_car->my_car.car_x = 
						background->w -20 +offsetX;
				}
		}
		
		/*
		(one_car->my_car.rotation >= 230 && 
				one_car->my_car.rotation < 310) )
			offsetY = 0;
		else
		{	
		}
		
		if(one_car->my_car.car_x < 20 - offsetX)
			one_car->my_car.car_x = 20 - offsetX;

		if(one_car->my_car.car_y < 20 - offsetY)
			one_car->my_car.car_y = 20 - offsetY;

		if(one_car->my_car.car_x > background->w -20+offsetX)
			one_car->my_car.car_x=background->w-20+offsetX;
		
		if(one_car->my_car.car_y > background->h -20+offsetY)
			one_car->my_car.car_y=background->h-20+offsetY;
		*/
		one_car = one_car->next;
	}
}



void carCollision (
		struct cars *car1, 
		struct cars *car2, 
		SDL_Surface *background, 
		int millitime)
{
	/*float piradian, sin_a, cos_a;

	double dplX, dplY;*/

	float ang;

	if(car1 == NULL || car2 == NULL)
	{
		return;
	}

	float rx = 0, ry = 0;
	if( find_displacement(car1, car2, &rx, &ry) == 0) {
		car1->my_car.car_x -= rx/2;
		car1->my_car.car_y -= ry/2;
		car2->my_car.car_x += rx/2;
		car2->my_car.car_y += ry/2;
	}

	return;
	
	if(car1 != NULL && car2 != NULL)
	{
		/*car1->my_car.collision = 1;
		car2->my_car.collision = 1;
		*/
		ang = collisionGetAngle(
				car1->my_car.car_x,
				car1->my_car.car_y,
				car2->my_car.car_x,
				car2->my_car.car_y);
		

		
		moveCar(&(car1->my_car), 
				-cos(ang/ARAD),
				-sin(ang/ARAD),
				background);

		if(collisionDoNotBreak(
			car1->my_car.rotation, ang, 0.25) )
				car_move_bk(&(car1->my_car), millitime );

		moveCar(&(car2->my_car),
				cos(ang/ARAD),
				sin(ang/ARAD),
				background);

		if(collisionDoNotBreak(
			car2->my_car.rotation, ang, 0.25) )
				car_move_bk(&(car2->my_car), millitime );
		/*
		car1->my_car.car_x -= car1->my_car.velx;
		car1->my_car.car_y -= car1->my_car.vely;
			
		car2->my_car.car_x -= car2->my_car.velx;
		car2->my_car.car_y -= car2->my_car.vely;
		
		car1->my_car.velx = 0.0f;
		car1->my_car.vely = 0.0f;
		car2->my_car.velx = 0.0f;
		car2->my_car.vely = 0.0f;
		*/
		/*
		if(car1->my_car.speed > car2->my_car.speed)
		{
			piradian = (car1->my_car.rotation)*M_PI/180.0;
	
			sin_a = sin(piradian);
			cos_a = cos(piradian);
	
			dplX = cos_a *(car1->my_car.speed);
			dplY = sin_a *(car1->my_car.speed);
			
			car2->my_car.speed = car1->my_car.speed;

			car1->my_car.speed = 0;
			
			car2->my_car.car_x += dplX;
			car2->my_car.car_y += dplY;
				
		}
		else
		{
			car1->my_car.speed = car2->my_car.speed;
			car2->my_car.speed = 0;
		}
		*/
	}
}

