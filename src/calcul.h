#ifndef __CALCUL_H__
#define __CALCUL_H__

void rotated_xy(float x, float y, float rotation, float *tx, float *ty);

void get_point_on_segment(struct points* p, struct points* q, float *x, float *y);

int min(double a, double b);

int max(double a, double b);

#endif
