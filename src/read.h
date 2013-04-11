#ifndef __READ_H__
#define __READ_H__

#include "map_struct.h"

struct mapData * readFileMapInfo(FILE *ptr_file);

/*void read_map_info(
	int *best_lap, 
	int *creator_best_lap, 
	const char *map_name,
	struct cars **car_player,
	int *total_checkpoints);*/

struct mapData * readMapTxt(char *mapName, int type);

#endif
