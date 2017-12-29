#include <math.h>

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

void rotated_xy(float x, float y, float rotation, float *tx, float *ty)
{
        float rad = rotation * M_PI/180.0;
	*tx = x * cos(rad) - y * sin(rad);
	*ty = x * sin(rad) + y * cos(rad);
}

void get_point_on_segment(struct points* p, struct points* q, float *x, float *y)
{
        float m1 = 0;
        float m2 = 0;
        float b = 0;
        if (p == NULL || q == NULL) {
                return;
        }

        // we need to find point on AB (pq)
        // to do so, first we need to find m and b 
        // for y = mx + b using the two points we 
        // already know (p and q)
        m1 = (q->y - p->y) / (q->x - p->x);

        // If the denominator is 0, m is unknown
        // but we know OX will be passing through 
        // two points: (0, 0) and (p->x, 0)
        // since y = 0
        if (q->x == p->x) {
                *x = p->x;
                *y = 0;
                return;
        }

        // b = y - mx
        b = p->y - m1 * p->x;

        // Now, using AB slope, we will calculate
        // the slope of the perpendicular OX
        // by invertng m
        m2 = -(q->x - p->x) / (q->y - p->y);

        // Using the perpendicular OX, find
        // the point crossing AB by looking for
        // the x first

        // If m1 was 0, m2 will result in -inf
        // so we need an alternative.
        // Given we know AB is vertical,
        // the perpendicular OX will be horizontal
        // which means it only need x
        // and we want the line that pass through 
        // the origin, x is equal to 0
        if (q->y == p->y) {
                *x = 0;
                *y = b;
        } else {
                *x = -b / (m1 - m2);
                // Using this value, we can calculate the y
                *y = m2 * *x;
        }
}


