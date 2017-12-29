#include <math.h>

void rotated_xy(float x, float y, float rotation, float *tx, float *ty) {
        float rad = rotation * M_PI/180.0;
	*tx = x * cos(rad) - y * sin(rad);
	*ty = x * sin(rad) + y * cos(rad);
}

int min(double a, double b)
{
	if(a<b)
		return(a);
	return(b);
}

int max(double a, double b)
{
	if(a>b)
		return(a);
	return(b);
}

