#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "config.h"

#include "struct_car.h"
#include "carlist.h"
#include "image.h"
#include "map_struct.h"
#include "debug.h"

#define MAP_FILE_TXT_FORMAT DATA_DIR"/maps/static/%s.txt"
#define MAP_FILE_CUSTOM_FORMAT DATA_DIR"/maps/custom/%s.txt"

struct mapData * readFileMapInfo(FILE *ptr_file)
{
	char read_line[256], 
	     /*car_img[256] = "cars/ferrariyellow.gif\0",*/
	     *get_line = NULL;
	struct car_properties *one_player = NULL;
	struct cars *top_properties = NULL;
	int player_number = 0;
	SDL_Surface *image = NULL;

	
	struct mapData *mpDt = NULL;

	mpDt = (struct mapData *)malloc( sizeof(struct mapData) );

	if(mpDt == NULL)
		return(NULL);
	
	mpDt->p = NULL;
	
	mpDt->mpBg = NULL;
	mpDt->mpCp = NULL;
	mpDt->mpRoad = NULL;
	mpDt->mpDecoration = NULL;
	mpDt->mpCollision = NULL;

	mpDt->hudSpeed = NULL;
	mpDt->hudLaps = NULL;
	
	mpDt->screen = NULL;

	mpDt->cmap = NULL;

	

	do
	{
		get_line = fgets(read_line,256,ptr_file);
		
		if(read_line[strlen(read_line)-1] == '\n')
		{
			read_line[strlen(read_line)-1] = '\0';
		}
		if(strlen(read_line) < 10 )
			continue ;
			
		if(0 == strncmp(read_line, "map_name=", 9))
		{
			/*printf("map name: %s\n",&(read_line[9]) );*/
		}
		
		if(0 == strncmp(read_line, "checkpts=", 9))
		{
			mpDt->totalCheckpoints = atoi(&(read_line[9]) );
		}
		
		if(0 == strncmp(read_line, "best_lap=", 9))
		{
			mpDt->bestLapTime = atoi(&(read_line[9]));
		}

		if(0 == strncmp(read_line, "gold_lap=", 9))
		{
			mpDt->goldenLapTime = atoi(&(read_line[9]));
		}
		
		if(0 == strncmp(read_line, "player", 6))
		{
			if(player_number < atoi(&(read_line[6])) || 
			player_number == 0)
			{
				player_number++;
				top_properties = add_car(&(mpDt->p));
				
				if(top_properties == NULL)
					break;
				
				if(CODEDEBUG)
				{
					printf(" - readFileMapInfo() : the map data.txt file presents a player that need to be added to the car list.\n");
				
					if(top_properties->prev == NULL)
						printf("There was no car in the list before now.\n");
					else
						printf("This is a not the first car added in the list.\n");
				}
				
				one_player = &(top_properties->my_car);

				if(one_player != NULL)
				{
					
					/*one_player -> best_lap = mpDt->bestLapTime;*/
					
				}
			}
			
			if(one_player != NULL)
			{
				if(0 == strncmp(&(read_line[7]), "x=", 2))
				{
					one_player->car_x = atof(&(read_line[9]));
					one_player->last_checkpoint_x = one_player->car_x;
				}

				if(0 == strncmp(&(read_line[7]), "y=", 2))
				{
					one_player->car_y = atof(&(read_line[9]));
					one_player->last_checkpoint_y = one_player->car_y;
				}

				if(0 == strncmp(&(read_line[7]), "r=", 2))
				{
					one_player->rotation = (float)atof(&(read_line[9]));
					one_player->last_checkpoint_rotation = 
						one_player->rotation;
				}
			}
		}
	} while(get_line != NULL);
	
	return(mpDt);
	
}

struct mapData * readMapTxt(char *mapName, int type)
{
	FILE *fp = NULL;
	
	char *fileName = NULL;

	struct mapData *r = NULL;
	
	if(mapName == NULL)
		return (NULL);
	
	if(strlen(mapName) > 200)
		return(NULL);

	if(type != 0 && type != 1)
		return(NULL);

	fileName = (char*) malloc(sizeof(char) *
			( strlen(mapName)+strlen(MAP_FILE_TXT_FORMAT) ) );

	if(fileName != NULL)
	{
		if(type == 0)
			sprintf(fileName, MAP_FILE_TXT_FORMAT, mapName);
		else
			if(type == 1)
				sprintf(fileName, MAP_FILE_CUSTOM_FORMAT, mapName);
				
		/*strcpy(fileName, "maps/infos/");
		strcat(fileName, mapName);
		strcat(fileName, post);
		*/
		
		fp = fopen(fileName,"r");
		
		if(fp != NULL)
		{
			r = readFileMapInfo(fp);
			
			fclose(fp);
		}
		else
		{
			fprintf(stderr,
				"Unable to read map data file: %s\n",
				fileName );
		}
	
		
		
		free(fileName);
		fileName = NULL;
	}

	return(r);

	
}

/*void read_map_info(
	int *best_lap, 
	int *creator_best_lap, 
	const char *map_name,
	struct cars **car_player,
	int *total_checkpoints)

{
	FILE *ptr_file = NULL;
	char read_line[256], 
	     file_name[256] = "maps/infos/", 
	     car_img[256] = "cars/ferrariyellow.gif\0",
		*get_line = NULL;
	struct car_properties *one_player = NULL;
	struct cars *top_properties = NULL;
	int player_number = 0;
	SDL_Surface *image = NULL;
	
	
	if(car_player == NULL)
	{
		fprintf(stderr,"Error: unable to find the list of cars.\n");
		return;
	}

	if(map_name == NULL)
		return;
	
	if(strlen(map_name) > 200)
		return;
	
	
	strcat(file_name,map_name);
	strcat(file_name,".txt");
	
	ptr_file = fopen(file_name,"r");
	
	if(ptr_file == NULL)
	{
		fprintf(stderr, 
			"Unable to read map info file: %s\n",
			file_name );
		return;
	}
	
	do
	{
		get_line = fgets(read_line,256,ptr_file);
		
		if(read_line[strlen(read_line)-1] == '\n')
		{
			read_line[strlen(read_line)-1] = '\0';
		}
		if(strlen(read_line) < 10 )
			continue ;
		if(0 == strncmp(read_line, "map_name=", 9))
		{
		}
		
		if(0 == strncmp(read_line, "checkpts=", 9))
		{
			*total_checkpoints = atoi(&(read_line[9]) );
		}
		
		if(0 == strncmp(read_line, "best_lap=", 9))
		{
			*best_lap = atoi(&(read_line[9]));
		}

		if(0 == strncmp(read_line, "gold_lap=", 9))
		{
			*creator_best_lap = atoi(&(read_line[9]));
		}
		
		if(0 == strncmp(read_line, "player", 6))
		{
			if(player_number < atoi(&(read_line[6])) || 
			player_number == 0)
			{
				player_number++;
				top_properties = add_car(car_player);
				
				if(top_properties == NULL)
					break;

				one_player = &(top_properties->my_car);
				
				one_player -> best_lap = *best_lap;
				
				if(player_number > 1)
					strcpy(car_img,"cars/porschegreen.gif\0");
				
				image = Load_img_from(car_img);
				if(image == NULL)
				{
					fprintf(stderr, "Error reading img\n");
					break;
				}
				if(image != NULL)
				{
					one_player->car = 
						SDL_DisplayFormatAlpha(image);
					
					SDL_FreeSurface(image);
					image = NULL;

					if(one_player -> car == NULL)
					{
						fprintf(stderr, 
						   "Error allocating img\n.");
						break;
					}
				}
			}
			
			if(0 == strncmp(&(read_line[7]), "x=", 2))
			{
				one_player->car_x = atof(&(read_line[9]));
				one_player->last_checkpoint_x = one_player->car_x;
			}

			if(0 == strncmp(&(read_line[7]), "y=", 2))
			{
				one_player->car_y = atof(&(read_line[9]));
				one_player->last_checkpoint_y = one_player->car_y;
			}

			if(0 == strncmp(&(read_line[7]), "r=", 2))
			{
				one_player->rotation = (float)atof(&(read_line[9]));
				one_player->last_checkpoint_rotation = 
					one_player->rotation;
			}
		}
	} while(get_line != NULL);
	
	fclose(ptr_file);
	
}*/
