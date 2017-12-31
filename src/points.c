#include <stdlib.h>
#include <stdio.h>

#include "struct_points.h"

struct points * init_point(float x, float y) {
        struct points *new_points = NULL;
        new_points = (struct points *) malloc( sizeof(struct points) );
        new_points->x = x;
        new_points->y = y;
        new_points->next = NULL;
        return new_points;
}

void free_points(struct points** p) {
        struct points *tmp = NULL;
        if (p == NULL)
        {
                return;
        }
        tmp = *p;
        while(tmp != NULL)
        {
                *p = tmp->next;
                free(tmp);
                tmp = *p;
        }
}

