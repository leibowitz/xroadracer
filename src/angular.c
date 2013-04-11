#include <math.h>

#include "angular.h"

void radius_rad( 
	float *calcul_angle, float rotation, 
	double * sin_a, double * cos_a)
{
	*calcul_angle = rotation * M_PI;
	*calcul_angle = (*calcul_angle)/180.0;
	*sin_a = sin(*calcul_angle);
	*cos_a = cos(*calcul_angle);
		return;
}


