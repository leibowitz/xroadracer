#ifndef __CALCUL_H__
#define __CALCUL_H__

#include "struct_points.h"
#include "struct_car.h"

void mk_minimum_displacement(struct points *p, float *rx, float *ry);

int find_displacement(struct cars *car1, struct cars *car2, float *rx, float *ry);

void rotated_xy(float x, float y, float rotation, float *tx, float *ty);

void get_point_on_segment(struct points* p, struct points* q, float *x, float *y);

struct points * minkowski_difference(struct points *p1, struct points *p2);

struct points * convex_hull(struct points *p);

struct points * convex_next(struct points *p, struct points *list);

float orientation(struct points *p, struct points *q, struct points *r);

struct points * get_coords(struct car_properties *car);

int min(double a, double b);

int max(double a, double b);

#endif
