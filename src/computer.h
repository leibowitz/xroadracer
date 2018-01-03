#ifndef __COMPUTER_H__
#define __COMPUTER_H__
#include "fann.h"
#include "map_struct.h"
#include "struct_car.h"

struct ai_key_output {
 struct fann *ann_fw;
 struct fann *ann_bw;
 struct fann *ann_right;
 struct fann *ann_left;
 struct fann *ann_fw_right;
 struct fann *ann_fw_left;
 struct fann *ann_bw_right;
 struct fann *ann_bw_left;
 struct fann *ann_none;
};

struct ai_data {
 struct ai_key_output* road;
 struct ai_key_output* offroad;
};

int computerIa(struct car_properties *p, struct mapData *mapInfos);

struct ai_data* load_ai_data();

void clear_ai_data(struct ai_data* data);

void print_output(int *keys, Uint8 *k);

#endif
