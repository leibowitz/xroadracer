#ifndef __MENU_H__
#define __MENU_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

enum {MENU_PRINCIPAL_SINGLE, MENU_PRINCIPAL_MULTI};
enum {
	MENU_SINGLE_1, 
	MENU_SINGLE_2, 
	MENU_SINGLE_3, 
	MENU_SINGLE_4, 
	MENU_SINGLE_5, 
	MENU_SINGLE_6, 
	MENU_SINGLE_7, 
	MENU_SINGLE_8, 
	MENU_SINGLE_9, 
	MENU_SINGLE_10, 
	MENU_SINGLE_11, 
	MENU_SINGLE_12};
	


int showAnyMenu(
	SDL_Surface *screen, 
	TTF_Font *font,
	char **tab_menu,
	int last_item_index);

void menu_item_move_up(
	int *current, 
	int total);

void menu_item_move_down(
	int *current, 
	int total);

void paint_menu_text(
	char *text, 
	TTF_Font *font, 
	SDL_Surface *screen, 
	int num_index, 
	int x, 
	int y);

void paint_menu_items(
	char **tab_menu_items, 
	int max, 
	int current, 
	TTF_Font *font, 
	SDL_Surface *screen, 
	int base_x, 
	int base_y, 
	int diff_y);

void menu_draw_selection(
	SDL_Surface *screen, 
	SDL_Surface *background, 
	int selection, 
	int base_x, 
	int base_y, 
	int diff_y, 
	char **tab_menu_items, 
	TTF_Font *font, 
	char *selection_img);

int listen_menu_event(
	int max_item_nb, 
	const int default_esc, 
	char **tab_menu_items,
	TTF_Font *font, 
	SDL_Surface *screen, 
	SDL_Surface *background);

char ** menu_alloc(int nb_item);

int countFileInDir(char *dir_name);

int returnFileInDir(char *dir_name, char **tab, int max);

int show_custommap_menu(
	SDL_Surface *screen, 
	char *map_name, 
	TTF_Font *font);

int show_play_menu(
	SDL_Surface *screen, 
	char *map_name, 
	TTF_Font *font);

int show_menu(
	SDL_Surface *screen, 
	char **map_name,
	int **players,
	TTF_Font *font);

int show_editor_menu(SDL_Surface *screen, TTF_Font *font);

void menuShowFromData(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack, SDL_Surface *menuImgSelector, char *fname);

int menuShow_Principal(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack, SDL_Surface *menuImgSelector);
int menuShow_Single(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack, SDL_Surface *menuImgSelector, char *mapName);
#endif
