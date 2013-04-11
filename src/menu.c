#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "config.h"

#include "main.h"
#include "menu.h"
#include "image.h"
#include "text.h"
#include "input.h"

#define MENU_TOPSTART 50
#define MENU_YSPACE 15

#define MENUBG_IMG DATA_DIR"/images/menu_background_color.gif\0"
#define MENUSELECT_IMG DATA_DIR"/images/menu_selection.gif\0"
#define CUSTOM_PATH DATA_DIR"/maps/custom"

int showAnyMenu(
	SDL_Surface *screen, 
	TTF_Font *font,
	char **tab_menu,
	int last_item_index)
{
	int 
		current = 0;
	
	SDL_Surface *background;

	background = show_splash(
		screen, 
		MENUBG_IMG);

	current = listen_menu_event(
		last_item_index, 
		-1, 
		tab_menu, 
		font, 
		screen, 
		background);
	
	SDL_FreeSurface(background);
	
	background = NULL;
	
	return(current);
}

void menu_item_move_up(
	int *current, 
	int total)
{
	if(*current > 0)
		(*current)--;
	else
		(*current) = total;
}

void menu_item_move_down(
	int *current, 
	int total)
{
	if(*current < total)
		(*current)++;
	else
		(*current) = 0;
}

void menuPrintText(SDL_Surface *screen, TTF_Font *font, char *text,
		SDL_Rect dest, SDL_Surface *menuImgSelector)
{
	SDL_Surface *tSurf;

	SDL_Rect clip = dest;

	enum txtMenuAlign { TX_CENTER, TX_LEFT } TXT_MENU_ALIGN = TX_CENTER;

	int txtSideSpace = 2;

	clip.x = 0;
	clip.y = 0;


	clip.w -= txtSideSpace * 2;

	drawtext(
			font,
			&tSurf,
			250, 250, 250,	250,
			0, 0, 0, 	0,
			text,
			TBLENDED);

	if(tSurf != NULL)
	{
		if(TXT_MENU_ALIGN == TX_CENTER)
		{
			if(clip.w > tSurf->w)
				clip.w = tSurf->w;
			
			dest.x += (menuImgSelector->w - clip.w) / 2;
		}
		else	
			if(TXT_MENU_ALIGN == TX_LEFT)
				dest.x += txtSideSpace;

		if(clip.h > tSurf->h)
			clip.h = tSurf->h;

		dest.y += (menuImgSelector->h - clip.h) / 2 ;


		SDL_BlitSurface(tSurf, &clip, screen, &dest);
		
		
		SDL_FreeSurface (tSurf);

		tSurf = NULL;
	}
}


void paint_menu_text(
	char *text, 
	TTF_Font *font, 
	SDL_Surface *screen, 
	int num_index, 
	int x, 
	int y)
{
	SDL_Surface *text_surface;
	SDL_Rect font_backg = {0,0,1,1};


	font_backg.x = x;
	font_backg.y = y;

	drawtext(
		font, 
		&text_surface,
		
		250,250,250, 		
		250,
		
		
		0, 0, 0, 		
		0,
		
		text,
		2);

	if(text_surface == NULL)
		return;
	
	SDL_BlitSurface(
		text_surface, 
		NULL, 
		screen, 
		&font_backg);

	SDL_UpdateRect(
		screen, 
		font_backg.x, 
		font_backg.y,
		font_backg.w, 
		font_backg.h);

	SDL_FreeSurface(text_surface);
	
	text_surface = NULL;
}

void paint_menu_items(
	char **tab_menu_items, 
	int max, 
	int current, 
	TTF_Font *font, 
	SDL_Surface *screen, 
	int base_x, 
	int base_y, 
	int diff_y)
{
	int iBcl, y;
	
	if(tab_menu_items == NULL)
		return;

	/*show_splash(screen, "images/menu_background_color.gif\0");*/
	
	for(iBcl = 0; iBcl <= max; iBcl++)
	{
		y = base_y + diff_y*iBcl;
		
		if(iBcl == current)
		{
			/*paint_menu_text(">", font, screen,
			0, base_x-10, y );*/
			
			show_img_xy(
				screen, 
				MENUSELECT_IMG,
				(base_x-10), 
				(y-7) );
		}
		
		paint_menu_text(
			tab_menu_items[iBcl], 
			font, 
			screen, 
			iBcl, 
			base_x, 
			y);
			
		/*printf("%s",tab_menu_items[iBcl]);*/
		
		/*	printf("*");
		printf("\n");*/
	}

	/*SDL_UpdateRect(screen, 0, 0,
			SCREEN_WIDTH-10, SCREEN_HEIGHT-10);*/
}

void menu_draw_selection(
	SDL_Surface *screen, 
	SDL_Surface *background, 
	int selection, 
	int base_x, 
	int base_y, 
	int diff_y, 
	char **tab_menu_items, 
	TTF_Font *font, 
	char *selection_img)
{
	SDL_Rect 
		clip_text, 
		clip_selection_img;
	
	if(tab_menu_items == NULL)
		return;

	clip_text.x = base_x;
	clip_text.y = base_y + diff_y*selection;
	
	clip_selection_img.x = clip_text.x - 10;
	clip_selection_img.y = clip_text.y - 7;
	
	clip_selection_img.w = 105;
	clip_selection_img.h = 27;
	
	if(selection_img != NULL)
	{
		show_img_xy(
			screen, 
			selection_img,
			clip_selection_img.x, 
			clip_selection_img.y);
	}
	else	
	{
		SDL_BlitSurface(
			background, 
			&clip_selection_img, 
			screen, 
			&clip_selection_img);
	}

	paint_menu_text(
		tab_menu_items[selection], 
		font, 
		screen, 
		selection, 
		clip_text.x, 
		clip_text.y);

	SDL_UpdateRect(
		screen, 
		clip_selection_img.x, 
		clip_selection_img.y, 
		clip_selection_img.w, 
		clip_selection_img.h);
}

void menuDrawSelector(SDL_Surface *screen, SDL_Surface *menuImgSelector, SDL_Rect *dest)
{
	

	SDL_BlitSurface (menuImgSelector, NULL, screen, dest);

	
}

void menuPrintAllText(
		SDL_Surface *screen, 
		TTF_Font *font, 
		char **tab_menu, 
		int max, 
		SDL_Rect *UpLeft,
		SDL_Surface *menuImgSelector,
		int menuSpaceY,
		int menuOffsetY)
{
	int bcl;

	SDL_Rect dest = *UpLeft;

	for(bcl = 0; bcl < max; bcl++)
	{
		dest.y = -menuOffsetY + UpLeft->y+ (menuImgSelector->h+menuSpaceY) * bcl;
		
		menuPrintText(screen, font, tab_menu[bcl], dest, menuImgSelector);
	}
	
	
}

/*
void menuPrintLine(SDL_Surface *screen, TTF_Font *font, char *text, SDL_Rect *dest)
{
}
*/

void menuCheckOffset(
		int current, 
		int *offset, 
		SDL_Surface *menuImgSelector,
		int menuSpaceY,
		SDL_Surface *screen,
		int max)
{
	int y = 0, bcl = 0;

	for(bcl = 0; bcl < current; bcl++)
	{
		y = (menuImgSelector->h+menuSpaceY) * (bcl+1);
	}

	/*printf("y %d offset %d\n", y, *offset);
	printf("screen h %d - menuImg h %d - menu Space Y %d\n",screen->h,
			menuImgSelector->h,
			menuSpaceY);*/


	if((y - *offset + MENU_TOPSTART) > (screen->h -menuImgSelector->h-menuSpaceY))
	{
		/**offset = (y-*offset) - (screen->h-menuImgSelector->h-menuSpaceY) ;*/
		if(current == max-1)
		{
			printf("LAST ITEM %d-",current);

			*offset = (menuImgSelector->h+menuSpaceY) * 
				(current+1 - (screen->h-MENU_TOPSTART) /
				 (menuImgSelector->h+menuSpaceY) );
			if(*offset < 0)
				*offset = 0;
		}
		else
			*offset += menuImgSelector->h+menuSpaceY;

		
		printf("menuoffsetDOWN %d\n",*offset);
	}
	else
	{
		if((y-*offset) < 0)
		{
			/*if(*offset >= menuImgSelector->h+menuSpaceY)
				*offset -= (menuImgSelector->h+menuSpaceY);
			else
				*offset = 0;*/

			if(y == 0)
			{
				printf("FIRST ITEM -");
				*offset = 0;
			}
			else
				*offset -= (menuImgSelector->h+menuSpaceY);
			

			printf("menuoffsetUP %d\n",*offset);
		}
	}
}

int menuKeyLoop(
		SDL_Surface *screen, 
		TTF_Font *font, 
		SDL_Joystick *joystick1,
		char **tab_menu, 
		int max, 
		SDL_Surface *menuImgBack, 
		SDL_Surface *menuImgSelector)
{
	/* ---------------------------------- */
	int loop = 1, 
	    currentSelection = 0,
	    lastSelection = 0;

	SDL_Event event, tmp_event;

	Uint8 *keys = NULL;

	
	int menuSpaceY = MENU_YSPACE,
	    menuOffsetY = 0;

	SDL_Rect position = { (screen->w-menuImgSelector->w)/2, MENU_TOPSTART, menuImgSelector->w, menuImgSelector->h };

	SDL_Rect dest = position;
	/* ---------------------------------- */
	if(max > 0)
	{
		SDL_BlitSurface (menuImgBack, NULL, screen, NULL);

		dest.y = position.y - menuOffsetY;

		menuDrawSelector(screen, menuImgSelector, &dest);


		menuPrintAllText(
				screen, 
				font, 
				tab_menu, 
				max, 
				&dest, 
				menuImgSelector, 
				menuSpaceY,
				menuOffsetY);

		SDL_Flip(screen);
		
		while(loop == 1)
		{
			keys = SDL_GetKeyState(NULL);
		
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_KEYDOWN:
					switch( (int) event.key.keysym.sym )
					{
						case SDLK_UP: 
						menu_item_move_up(
							&currentSelection,
							(max-1));
						menuCheckOffset(currentSelection,
							&menuOffsetY,
							menuImgSelector,
							menuSpaceY,
							screen,
							max);
						break;

						case SDLK_DOWN: 
						menu_item_move_down(
							&currentSelection,
							(max-1));
						menuCheckOffset(currentSelection,
							&menuOffsetY,
							menuImgSelector,
							menuSpaceY,
							screen,
							max);
						break;

						case SDLK_RETURN:
						case SDLK_KP_ENTER:
						case SDLK_RIGHT:
						loop = 0;
						break;

						case SDLK_ESCAPE:
						case SDLK_BACKSPACE:
						case SDLK_LEFT:
						tmp_event.type = SDL_QUIT;
						SDL_PushEvent(&tmp_event);
						break;

						/* Bad way of switching to fs 
						case SDLK_f:
						SDL_WM_ToggleFullScreen(screen);
						break;
						*/
					}
					break;
				
					case SDL_QUIT:
					loop = 0;
					currentSelection = -1;
					break;
				}
			}
		
			/* If the selection has changed, please redraw the revelant screen part */
			if(currentSelection != lastSelection && loop == 1)
			{
				/* refresh only the last selection part */
				/* Clear the last selection Selector area */
				/*SDL_BlitSurface (menuImgBack, &dest, screen, &dest);*/
				SDL_BlitSurface (menuImgBack, NULL, screen, NULL);
				
				/* reprint the text there */
				/*menuPrintText(screen, font, tab_menu[lastSelection], dest, menuImgSelector);

				SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);*/
				
				if(currentSelection >= 0 && currentSelection < max)
				{

					/* move the Selection Image to the current menu item */
					dest.y = MENU_TOPSTART + (menuImgSelector->h + menuSpaceY) * currentSelection - menuOffsetY;


					menuDrawSelector(screen, menuImgSelector, &dest);
					
					/*menuPrintText(screen, font, tab_menu[currentSelection], dest, menuImgSelector);
					
					SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);*/
				}
				
				menuPrintAllText(
					screen, 
					font, 
					tab_menu, 
					max, 
					&position, 
					menuImgSelector, 
					menuSpaceY,
					menuOffsetY);
				
				
				SDL_Flip(screen);

				
			}

			lastSelection = currentSelection;
		}
	}

	return(currentSelection);
}


int listen_menu_event(
	int max_item_nb, 
	const int default_esc, 
	char **tab_menu_items,
	TTF_Font *font, 
	SDL_Surface *screen, 
	SDL_Surface *background)
{
	/* ------------------------------------------- */
	int 
		loop = 1, 
		current_selection = 0, 
		last_selection = 1,
		move = 0,
		last_move = 0;
	
	SDL_Event 
		event, 
		tmp_event;
		
	Uint8* keys = NULL;
	
	/* ------------------------------------------- */

	if(max_item_nb == 0)
		return(current_selection);
	
	if(max_item_nb > 9)
		return(current_selection);
	
	paint_menu_items(
		tab_menu_items, 
		max_item_nb,
		current_selection, 
		font, 
		screen,
		40, 
		50,
		30);


	while(loop == 1)
	{
		keys = SDL_GetKeyState(NULL);
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch( (int) event.key.keysym.sym )
					{
						case SDLK_UP: 
						move = 1;
						menu_item_move_up(
							&current_selection,
							max_item_nb);
						break;

						case SDLK_DOWN: 
						move = -1;
						menu_item_move_down(
							&current_selection,
							max_item_nb);
						break;

						case SDLK_RETURN:
						case SDLK_KP_ENTER:
						loop = 0;
						break;

						case SDLK_ESCAPE:
						tmp_event.type = SDL_QUIT;
						SDL_PushEvent(&tmp_event);
						break;

						/* Bad way of switching to fs 
						case SDLK_f:
						SDL_WM_ToggleFullScreen(screen);
						break;
						*/
					}
				break;

				case SDL_KEYUP:
				switch( (int) event.key.keysym.sym )
				{
					case SDLK_UP:
					case SDLK_DOWN: 
					move = 0;
					break;
				}
				break;
				
				case SDL_QUIT:
				loop = 0;
				current_selection = default_esc;
				break;
				
			}
		}
		/*
		if(move > 0)
			menu_item_move_up(
				&current_selection,
				max_item_nb);
		else if(move < 0)
			menu_item_move_down(
				&current_selection,
				max_item_nb);
		*/
		if(current_selection != last_selection)
		{
			if(last_selection >= 0)
			menu_draw_selection(
				screen,
				background,
				last_selection, 
				40, 
				50, 
				30, 
				tab_menu_items, 
				font, 
				NULL);
			
			if(current_selection >= 0)
			menu_draw_selection(
				screen,
				background,
				current_selection, 
				40, 
				50, 
				30, 
				tab_menu_items, 
				font, 
				MENUSELECT_IMG);

		}
		/*
		if (loop == 1)
		{
			if(last_move != move)
			{
				SDL_Delay(200);
			}
			else
			{
				SDL_Delay(150);
			}
		}
		*/
		last_move = move;
		
		last_selection = current_selection;
		
	}

	return(current_selection);
}

char ** menu_alloc(int nb_item)
{
	char **tab_menu;
	
	tab_menu = (char**)malloc(sizeof(char*)*(nb_item));
	
	if(tab_menu == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		return(NULL);
	}

	return(tab_menu);
}

int countFileInDir(char *dir_name)
{
	DIR *df;
	struct dirent *info;
	
	int nb = 0;

	df = opendir(dir_name);

	if(df != NULL)
	{
		do
		{
			info = readdir(df);
			
			if(info != NULL)
			{
				if(strcmp(info->d_name, ".") != 0 && 
					strcmp(info->d_name, "..")!= 0)
					nb++;
			}

		} while (info != NULL);
		closedir(df);
	}
	
	return(nb);
}

int returnFileInDir(char *dir_name, char **tab, int max)
{
	DIR *df;
	struct dirent *info;
	
	int nb = 0;
	
	/*if(max > 0)
	{
		strcpy(tab[0], "hello");
		printf("tab 0 = %s\n",tab[0]);
	}*/

	df = opendir(dir_name);

	if(df != NULL)
	{
		do
		{
			info = readdir(df);
			
			if(info != NULL)
			{
				/*printf(" -|- %s\n",info->d_name);*/
				if(strcmp(info->d_name, ".") != 0 && 
					strcmp(info->d_name, "..")!= 0)
				{
					if(nb < max)
					{
						strcpy(tab[nb++],
							info->d_name);
					/*printf("%s (%s)\n",
					 	info->d_name,
						tab[nb++]);*/
					}
				}
			}

		} while (info != NULL);
		/*printf("return files done\n");*/
		closedir(df);
	}
	
	return(nb);
}
/*
int show_custom_map_menu(SDL_Surface *screen, 
		char *map_name, 
		TTF_Font *font)
{
	int nb = 0, current = 0;
	char **tab_menu = NULL;
	char img[] = "images/menu_background_color.gif";
	SDL_Surface *background;

	nb = countFileInDir("custom");


	if(nb < 1)
		return(-1);

	tab_menu = menu_alloc(nb);

	if(tab_menu == NULL)
		return(-1);
	for(current = 0; current < nb; current++)
		strcpy(tab_menu[current], "\0");
	returnFileInDir("custom", tab_menu, nb);
	for(current = 0; current < nb; current++)
	{
		printf("%d- ",current);
		printf(" %s\n",tab_menu[current]);
	}
	printf("bug here (menu.c FIXME)\n");
	current = showAnyMenu(screen, font, tab_menu, (nb-1));
	
	background = show_splash(
		screen, 
		img);
	if(background != NULL)
	{
		printf("pouetpouet-retapouet\n");
	current = listen_menu_event(
		(nb-1), 
		-1, 
		tab_menu, 
		font, 
		screen, 
		background);
	
	
		SDL_FreeSurface(background);
	
	background = NULL;
	}
	printf("clear tab menu\n");
	if(tab_menu != NULL)
		free(tab_menu);

	tab_menu = NULL;
	printf("quit\n");
	
	return(current);
}
*/
int show_custommap_menu(
	SDL_Surface *screen, 
	char *map_name, 
	TTF_Font *font)
{
	int 
		current = 0, 
		nb,
		bcl;
	
	char **tab_menu = NULL;
	
	/*char map_b_01[] = "grassroad\0";
	char map_b_02[] = "simsilver\0";*/
	
	nb = countFileInDir(CUSTOM_PATH);
	/*nb = 1;*/
		
	if(nb < 1)
		return(-1);
	
	/*SDL_Surface *background;
	*/
	if(map_name == NULL)
		return(-1);
		
	tab_menu = menu_alloc(nb);

	if(tab_menu == NULL)
		return(-1);

	/*background = show_splash(
		screen, 
		"images/menu_background_color.gif\0");
	*/
	
	for( bcl=0; bcl < nb; bcl++)
	{
		tab_menu[bcl] = (char *)malloc(sizeof(char)*200);
		/*strcpy(tab_menu[current],"abc\0");*/
	}
	
	returnFileInDir(CUSTOM_PATH, tab_menu, nb);
	
	/*for( bcl=0; bcl < nb; bcl++)
		printf(" - %s -\n",tab_menu[bcl]);*/
		
	current = showAnyMenu(screen, font, tab_menu, (nb-1));
	/*current = listen_menu_event(
		last_item_index, 
		-1, 
		tab_menu, 
		font, 
		screen, 
		background);
	*/
	if(current > -1)
	{
		sprintf(map_name, "%s", tab_menu[current] );
	}

	for(bcl = 0; bcl<nb; bcl++)
		free(tab_menu[bcl]);

	free(tab_menu);

	tab_menu = NULL;
	
	/*SDL_FreeSurface(background);
	
	background = NULL;*/
	
	return(current);
}

int show_play_menu_OBSOLETE(
	SDL_Surface *screen, 
	char *map_name, 
	TTF_Font *font)
{
	int 
		current = 0, 
		last_item_index = 7;
	
	char **tab_menu = NULL;
	
	char map_b_01[] = "simsilver\0";
	char map_b_02[] = "heightfing\0";
	char map_b_03[] = "longroad\0";
	char map_b_04[] = "softloop\0";
	char map_b_05[] = "speedway\0";
	char map_b_06[] = "waychoice\0";
	char map_b_07[] = "xaramap\0";
	char map_b_08[] = "mapdesigned1\0";

	
	/*SDL_Surface *background;
	*/
	if(map_name == NULL)
		return(-1);
		
	tab_menu = menu_alloc(last_item_index+1);

	if(tab_menu == NULL)
		return(-1);

	/*background = show_splash(
		screen, 
		"images/menu_background_color.gif\0");
	*/
	tab_menu[0] = map_b_01;
	tab_menu[1] = map_b_02;
	tab_menu[2] = map_b_03;
	tab_menu[3] = map_b_04;
	tab_menu[4] = map_b_05;
	tab_menu[5] = map_b_06;
	tab_menu[6] = map_b_07;
	tab_menu[7] = map_b_08;

	current = showAnyMenu(screen, font, tab_menu, last_item_index);
	/*current = listen_menu_event(
		last_item_index, 
		-1, 
		tab_menu, 
		font, 
		screen, 
		background);
	*/
	if(current > -1)
	{
		sprintf(map_name, "%s", tab_menu[current] );
	}

	free(tab_menu);

	tab_menu = NULL;
	
	/*SDL_FreeSurface(background);
	
	background = NULL;*/
	
	return(current);
}

void menuDataRead(FILE *fp)
{
	char buf[256];

	while (fgets ( buf, 256, fp) != NULL)
	{
		printf("%s", buf);
	}
	
}

void menuShowFromData(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack, SDL_Surface *menuImgSelector, char *fname)
{
	FILE *fp;
	
	
	if(fname != NULL)
	{
		fp = fopen(fname, "r");

		if(fp != NULL)
		{
			menuDataRead(fp);
			
			fclose(fp);
		}
		else
			fprintf(stderr, "Unable to load menu data file: %s\n", fname);
	}
	
}

int menuShow_Single(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack, SDL_Surface *menuImgSelector, char *mapName)
{
	
	/* ----------------------------- */
	char menu1[] = "map1";
	char menu2[] = "heightfing";
	char menu3[] = "longroad";
	char menu4[] = "softloop";
	char menu5[] = "speedway";
	char menu6[] = "waychoice";
	char menu7[] = "pro1";
	char menu8[] = "road1";
	char menu9[] = "urban1";
	char menu10[] = "Back";

	/*char player[50] = "";*/

	int code = -1;

	char **tab_menu = NULL;

	int tab[10];
	/* ----------------------------- */
	/*if( getKeyboard(menuImgBack, screen, font, player, 50, 
			"Please enter your name:") > 0 )*/
	{
		tab_menu = menu_alloc(10);

		if(tab_menu != NULL)
		{
			tab_menu[0] = menu1;
			tab_menu[1] = menu2;
			tab_menu[2] = menu3;
			tab_menu[3] = menu4;
			tab_menu[4] = menu5;
			tab_menu[5] = menu6;
			tab_menu[6] = menu7;
			tab_menu[7] = menu8;
			tab_menu[8] = menu9;
			tab_menu[9] = menu10;

			tab[0] = MENU_SINGLE_1;
			tab[1] = MENU_SINGLE_2;
			tab[2] = MENU_SINGLE_3;
			tab[3] = MENU_SINGLE_4;
			tab[4] = MENU_SINGLE_5;
			tab[5] = MENU_SINGLE_6;
			tab[6] = MENU_SINGLE_7;
			tab[7] = MENU_SINGLE_8;
			tab[8] = MENU_SINGLE_9;
			tab[9] = -1;

			code = menuKeyLoop(
					screen, font, joystick1, 
					tab_menu, 10, menuImgBack, 
					menuImgSelector);


			if(code >= 0 && code < 10)
			{
				if(tab[code] > -1)
					sprintf(mapName, "%s", tab_menu[code] );

				code = tab[code];
				
			}
			
			free(tab_menu);

			tab_menu = NULL;
		}
	}

	return(code);
}

int menuShow_Principal(SDL_Surface *screen, TTF_Font *font, SDL_Joystick *joystick1,
		SDL_Surface *menuImgBack, SDL_Surface *menuImgSelector)
{
	/* ----------------------------- */
	char menu1[] = "Single Player";
	char menu2[] = "Multiplayer";
	char menu3[] = "Quit";

	int code = 0;

	char **tab_menu = NULL;

	int tab[2/*3*/];
	/* ----------------------------- */

	tab_menu = menu_alloc(3);

	if(tab_menu != NULL)
	{
		tab_menu[0] = menu1;
		/*tab_menu[1] = menu2;*/
		tab_menu[1 /*2*/] = menu3;

		tab[0] = MENU_PRINCIPAL_SINGLE;
		/*tab[1] = MENU_PRINCIPAL_MULTI;*/
		tab[1 /*2*/] = -1;

		code = menuKeyLoop(
				screen, font, joystick1, 
				tab_menu, 2/*3*/, menuImgBack, 
				menuImgSelector);

		if(code >= 0 && code < 2/*3*/)
			code = tab[code];
		
		free(tab_menu);

		tab_menu = NULL;
	}

	return(code);
}
/*
int show_menu(
	SDL_Surface *screen, 
	char **map_name,
	int **players,
	TTF_Font *font)
{
	int 
		current = 0, 
		last_item_index = 2;
	
	char **tab_menu = NULL;
	
	char menu_play[] = "Play\0";
	
	char menu_quit[] = "Quit\0";

	char menu_custom[] = "Custom Map\0";
	
	SDL_Surface *background;

	tab_menu = menu_alloc(last_item_index+1);

	if(tab_menu == NULL)
		return(0);

	background = show_splash(
		screen, 
		"images/menu_background_color.gif\0");

	tab_menu[MENU_PLAY] = menu_play;
	tab_menu[MENU_CUSTOMMAP] = menu_custom;
	tab_menu[MENU_QUIT] = menu_quit;

	current = listen_menu_event(
		last_item_index, 
		MENU_QUIT, 
		tab_menu, 
		font, 
		screen, 
		background);
	
	free(tab_menu);

	tab_menu = NULL;
	
	SDL_FreeSurface(background);
	
	background = NULL;
	
	return(current);
}

int show_game_menu(
	SDL_Surface *screen, 
	SDL_Surface *background,
	TTF_Font *font
)
{
	int 
		current = 0, 
		last_item_index = 1;
	
	char **tab_menu = NULL;
	
	char option_01[] = "Continue\0";
	char option_02[] = "Quit\0";
	
	tab_menu = menu_alloc(last_item_index+1);

	if(tab_menu == NULL)
		return(-1);

	tab_menu[0] = option_01;
	tab_menu[1] = option_02;

	current = listen_menu_event(
		last_item_index, 
		0, 
		tab_menu, 
		font, 
		screen, 
		background);
	
	free(tab_menu);

	tab_menu = NULL;
	
	return(current);
}*/
int show_editor_menu(SDL_Surface *screen, TTF_Font *font)
{	
	int retour;
	char **tab_menu = NULL;
	tab_menu = menu_alloc(2);

	if(tab_menu == NULL)
		return(0);

	tab_menu[0] = "New";
	tab_menu[1] = "Quit";

	retour = showAnyMenu(screen,font, tab_menu, 1);
	
	free(tab_menu);
	tab_menu = NULL;
	return(retour);
}
