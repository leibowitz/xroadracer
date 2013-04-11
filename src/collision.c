#include <math.h>

#include "collision.h"

/* Collision code from trophy.sf.net  (1.1.3) 
 * (GPL v2 - Andrew Mustun) 
 * camath.cpp */
int collisionCompFloat(float f1, float f2, float tol)
{
	float delta = f2-f1;
	return( delta>-tol && delta<tol );
}

float collisionCorrAngle(float ang)
{
	float ret=ang;
	while(ret < 0.0) ret+=360.0;
	while(ret>=360.0) ret-=360.0;
	return ret;
}

float collisionGetAngle(float x1, float y1, float x2, float y2)
{
	float xdist, ydist, angle;

	xdist = x2-x1;
	ydist = y2-y1;

	if(collisionCompFloat(xdist, 0.0, 0.0)) {
		if(y2<y1) angle=270.0;
		else angle=90.0;
	} else if(collisionCompFloat(ydist, 0.0, 0.0)) {
		if(x2<x1) angle=180.0;
		else angle=0.0;
	} else {
		angle = atan(ydist/xdist)*ARAD;
		if(angle<0.0) angle*=-1.0;
		if(x2>x1 && y2>y1) angle = 0.0 + angle;
		if(x2<x1 && y2>y1) angle = 180.0 - angle;
		if(x2<x1 && y2<y1) angle = 180.0 + angle;
		if(x2>x1 && y2<y1) angle = 360.0 - angle;
	}

	return(angle);
}

int collisionDoNotBreak(float r1, float r2, float tol)
{
	float sin1, cos1, rad1, 
	      sin2, cos2, rad2;

	rad1 = r1/ARAD;
	rad2 = r2/ARAD;

	cos1 = cosf(rad1);
	sin1 = sinf(rad1);

	cos2 = cosf(rad2);
	sin2 = sinf(rad2);
	
	return( fabs(cos1-cos2) < tol && fabs(sin1-sin2) < tol );
}

void collision_OBSOLETE(struct cars *one_car)
{
	/* ------------------------------------------- */

	struct cars *thecar = one_car, *p1car;
	
	double vitessep1 = -0.1;
	
	float degrees;
	
	double sinus, cosinus;
	
	double car_front_x, car_front_y;
	
	double diff_x, diff_y;
	
	double car_next_x, car_next_y;
	
	double p1_next_x, p1_next_y;
	
	/* ------------------------------------------- */

	/*printf("TO-DO: start_game.c - collision reaction\n");*/
	
	while(thecar != NULL)
	{
		radius_rad(&degrees,thecar->my_car.rotation,&sinus,&cosinus);
		
		car_front_x = cosinus *20 + thecar->my_car.car_x;
		car_front_y = sinus *20 + thecar->my_car.car_y;
		
		diff_x = thecar->my_car.car_x - thecar->my_car.last_x;
		diff_y = thecar->my_car.car_y - thecar->my_car.last_y;
		
		car_next_x = car_front_x + diff_x;
		car_next_y = car_front_y + diff_y;
		
		if(vitessep1 < -0.05)
		{
			vitessep1 = thecar->my_car.speed;
			p1car  = thecar;
			
			p1_next_x = car_next_x;
			p1_next_y = car_next_y;
		}
		else
		{
			/* if p1 position in next frame is in p2 
			if(p1_next_x, p1_next_y, 	
			do 
			
			p1car->my_car.car_x = p1car->my_car.last_x;
			p1car->my_car.car_y = p1car->my_car.last_y;
			car_move_bk(&vitessep1, p1car );
			
			else
			
			thecar->my_car.car_x = thecar->my_car.last_x;
			thecar->my_car.car_y = thecar->my_car.last_y;
			car_move_bk(&vitessep1, thecar );
			
			*/
		}
		
		
		thecar = thecar->next;
	}
}
