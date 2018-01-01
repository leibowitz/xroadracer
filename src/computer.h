#ifndef __COMPUTER_H__
#define __COMPUTER_H__
#include "fann.h"
#include "map_struct.h"
#include "struct_car.h"
struct ai_data {
 struct fann *ann_fw;
 struct fann *ann_bw;
 struct fann *ann_right;
 struct fann *ann_left;
};

void computerIa(struct car_properties *p, struct mapData *mapInfos);

struct ai_data* load_ai_data();

void clear_ai_data(struct ai_data* data);

#endif
