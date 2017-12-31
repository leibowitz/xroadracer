#include <math.h>
#include <float.h>

#include "struct_points.h"
#include "struct_car.h"
#include "points.h"

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

struct points * get_coords(struct car_properties *car)
{
        if (car == NULL) {
                return NULL;
        }

        float xa, ya, xb, yb;
        float xc, yc, xd, yd;

        float x, y;
        x = car->car_x;
        y = car->car_y;

        float x1, x2, y1, y2;
        x1 = -car->car->h / 2;
        y1 = -car->car->w / 4;
        x2 = car->car->h / 2;
        y2 = car->car->w / 4;

        rotated_xy(x1, y1, car->rotation, &xa, &ya);
        xa += x;
        ya += y;
        rotated_xy(x1, y2, car->rotation, &xb, &yb);
        xb += x;
        yb += y;
        rotated_xy(x2, y2, car->rotation, &xc, &yc);
        xc += x;
        yc += y;
        rotated_xy(x2, y1, car->rotation, &xd, &yd);
        xd += x;
        yd += y;

        struct points *pa = init_point(xa, ya);
        struct points *pb = init_point(xb, yb);
        struct points *pc = init_point(xc, yc);
        struct points *pd = init_point(xd, yd);
        pa->next = pb;
        pb->next = pc;
        pc->next = pd;
        return pa;
}

float orientation(struct points *p, struct points *q, struct points *r)
{
        float val = (q->y - p->y) * (r->x - q->x) - (q->x - p->x) * (r->y - q->y);
        if (val == 0)
                return 0;
        if (val > 0)
                return 1;
        else
                return 2;
}

struct points * convex_next(struct points *p, struct points *list) {
        float tmp_ang = 0;
        float ang = 0;
        struct points *tmp = NULL;
        struct points *next = NULL;
        tmp = list;

        next = tmp;
        while (tmp != NULL) {
                if (tmp != list) {
                        if (next == p || orientation(p, tmp, next) == 2) {
                                next = tmp;
                        }
                }
                tmp = tmp->next;
        }

        return next;
}

struct points * convex_hull(struct points *p) {
        struct points *list = NULL;
        struct points *last = NULL;
        struct points *tmp = NULL;
        struct points *next = NULL;
        struct points *left_most = NULL;
        float x = FLT_MAX;
        tmp = p;

        // Find left most x
        while (tmp != NULL) {
                if (tmp->x < x) {
                        x = tmp->x;
                        left_most = tmp;
                }
                tmp = tmp->next;
        }

        // find next point
        next = left_most;
        int i = 0;
        do {
                if (next == NULL)
                {
                        break;
                }
                i++;
                if (i > 10) {
                        break;
                }

                tmp = init_point(next->x, next->y);

                if (list == NULL) {
                        list = tmp;
                }

                if (last != NULL) {
                        last->next = tmp;
                }

                last = tmp;

                next = convex_next(next, p);
        } while(next != NULL && next != left_most);

        return list;
}

struct points * minkowski_difference(struct points *p1, struct points *p2) {
        struct points *tmp = NULL;
        struct points *last = NULL;
        struct points *list = NULL;
        struct points *pi = NULL;
        struct points *pj = NULL;

        pi = p1;

        while (pi != NULL) {
                pj = p2;

                while (pj != NULL) {
                        tmp = init_point(pi->x - pj->x, pi->y - pj->y);

                        if (list == NULL) {
                                list = tmp;
                        }
                        if (last != NULL) {
                                last->next = tmp;
                        }

                        last = tmp;

                        pj = pj->next;
                }

                pi = pi->next;
        }

        return list;
}

void mk_minimum_displacement(struct points *p, float *rx, float *ry)
{
        struct points *n = p;
        struct points *tmp = p;
        float smallest_v = FLT_MAX;
        float v = 0;
        float x = 0, y = 0;
        int i = 0;

        while(tmp != NULL) {
                i++;
                if (tmp->next == NULL) {
                        if (tmp == p) {
                                break;
                        }
                        n = p;
                } else {
                        n = tmp->next;
                }

                get_point_on_segment(tmp, n, &x, &y);
                v = hypotf(x, y);

                if (smallest_v > v) {
                        smallest_v = v;
                        *rx = x;
                        *ry = y;
                }

                tmp = tmp->next;
        }
}

int find_displacement(struct cars *car1, struct cars *car2, float *rx, float *ry)
{
        if (car1 == NULL || car2 == NULL || car1 == car2) {
                return -1;
        }

        float x, y, w, h;
        x = car1->my_car.car_x - (car1->my_car.car->w/2) - (car2->my_car.car_x + (car2->my_car.car->w/2));
        y = car1->my_car.car_y - (car1->my_car.car->h/2) - (car2->my_car.car_y + (car2->my_car.car->h/2));
        w = car1->my_car.car->w + car2->my_car.car->w;
        h = car1->my_car.car->h + car2->my_car.car->h;
        
        struct points *p1 = NULL;
        struct points *p2 = NULL;
        p1 = get_coords(&car1->my_car);
        p2 = get_coords(&car2->my_car);
        struct points *diff = NULL;
        diff = minkowski_difference(p1, p2);
        
        struct points *hull = NULL;
        hull = convex_hull(diff);

        mk_minimum_displacement(hull, rx, ry);

        free_points(&p1);
        free_points(&p2);
        free_points(&diff);
        free_points(&hull);
        
        return 0;
}
