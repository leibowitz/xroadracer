#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "angular.h"


#include "struct_car.h"


int collisionCompFloat(float f1, float f2, float tol);

float collisionCorrAngle(float ang);

float collisionGetAngle(float x1, float y1, float x2, float y2);

int collisionDoNotBreak(float r1, float r2, float tol);

/*void collision(struct cars *one_car);*/

#endif
