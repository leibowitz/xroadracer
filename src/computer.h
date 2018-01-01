#ifndef __COMPUTER_H__
#define __COMPUTER_H__
#include "fann.h"
#include "map_struct.h"
#include "struct_car.h"
struct ai_data {
 struct fann *road_ann_fw;
 struct fann *road_ann_bw;
 struct fann *road_ann_right;
 struct fann *road_ann_left;
 
 struct fann *offroad_ann_fw;
 struct fann *offroad_ann_bw;
 struct fann *offroad_ann_right;
 struct fann *offroad_ann_left;
};

int computerIa(struct car_properties *p, struct mapData *mapInfos);

struct ai_data* load_ai_data();

void clear_ai_data(struct ai_data* data);

#endif
