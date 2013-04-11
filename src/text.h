#ifndef __TEXT_H__
#define __TEXT_H__

enum textquality {TSOLID, TSHADED, TBLENDED};

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "struct_car.h"
#include "map_struct.h"
/*
void clear_precedent_message(
	SDL_Surface *background,
	SDL_Surface *screen,
	SDL_Rect font_backg);

void print_message_on_screen(
	TTF_Font *font,
	SDL_Surface **text_surface,
	SDL_Rect *font_backg,
	SDL_Surface *screen,
	int x,int y,
	char *text,
	SDL_Surface *background);

*/
SDL_Rect textOnScreen(SDL_Surface *screen,
		char *message,
		TTF_Font *font,
		int x,
		int y,
		int hud,
		int align);

SDL_Rect print_hud_text(
	SDL_Surface *screen, 
	char *message, 
	int x, 
	int y, 
	TTF_Font *font,
	int hud);

void show_hud(
	SDL_Surface *screen, 
	TTF_Font *font, 
	struct cars *the_player,
	int pause,
	int max_tours,
	int fps,
	int hud,
	struct mapData *mapInfos);

void drawtext(
	TTF_Font *fonttodraw, SDL_Surface **destsurf, 
	char fgR, char fgG, char fgB, char fgA, 
	char bgR, char bgG, char bgB, char bgA, 
	char text[], int quality);

#endif
