#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "text.h"
#include "main.h"

#include "key.h"



SDL_Rect textOnScreen(SDL_Surface *screen,
		char *message,
		TTF_Font *font,
		int x,
		int y,
		int hud,
		int align)
{
	Uint32 color = SDL_MapRGB(screen->format, 0,0,0);
	
	SDL_Rect position = {x,y,1,1};
	
	SDL_Surface *text = NULL;

	
	drawtext(
		font, 
		&text, 
		255, 255, 255, 255, 
		0,0,0,0, 
		message, 
		2);
	
	if(text != NULL)
	{
		
		position.w = text->w;
		position.h = text->h;
		
		switch(align)
		{
			/* align 1 is center on x, y */
			case 1:
				position.x = x- text->w/2;
				position.y = y- text->h/2;
				break;

			/* and align 2 is up right */
			case 2:
				position.x = x-text->w;
				position.y = y;
				break;

			/* left bottom */
			case 3:
				position.x = x;
				position.y = y-text->h;
				break;
			
			/* x,y is right bottom */
			case 4: 
				position.x = x-text->w;
				position.y = y-text->h;
				break;

			/* x is on the center, y is on top */
			case 5:
				position.x = x-text->w/2;
				position.y = y;
				break;
				
			/* align 0 is up left position */
			default:
			position.x = x;
			position.y = y;
				break;
		}
		
		
		if(hud == 1)
			SDL_FillRect(screen, &position, color);
		

		SDL_BlitSurface(text, NULL, screen, &position);
		
		SDL_FreeSurface(text);
		
		text = NULL;
	}

	return(position);
}

SDL_Rect print_hud_text(
	SDL_Surface *screen, 
	char *message, 
	int x, 
	int y, 
	TTF_Font *font,
	int hud)
{
	return( textOnScreen(screen, message, font, x, y, hud, 0) );
}

void getMilliTime(Uint32 time, char *txt)
{

	sprintf(txt, "%d.%03d", (time/1000),(time%1000) );
}

void imgHud(struct mapData *mapInfos ,struct cars *car_player)
{
	SDL_Rect dest;

	dest.x = car_player->my_car.destView.x +
		car_player->my_car.destView.w -
		mapInfos->hudLaps->w-5;

	dest.y = car_player->my_car.destView.y+5;

	SDL_BlitSurface(
			mapInfos->hudLaps,
			NULL,
			mapInfos->screen,
			&dest);

	dest.x = car_player->my_car.destView.x +
		car_player->my_car.destView.w -
		mapInfos->hudSpeed->w-5;

	dest.y = car_player->my_car.destView.y +
		car_player->my_car.destView.h -
		mapInfos->hudSpeed->h-2;

	SDL_BlitSurface(
			mapInfos->hudSpeed,
			NULL,
			mapInfos->screen,
			&dest);

}

void textHudHuman(
		struct mapData *mapInfos,
		TTF_Font *font, 
		struct cars *car_player, 
		char *txt_msg, 
		int max_tours, 
		int hud)
	
{
	if(car_player->my_car.missed == 1)
	{
		sprintf(txt_msg,
			"You missed a checkpoint !");

		textOnScreen(mapInfos->screen,
			txt_msg,
			mapInfos->font,
				
			car_player->my_car.destView.x + 
			(car_player->my_car.destView.w/2),
			car_player->my_car.destView.y + 
			(car_player->my_car.destView.h/2),

			hud,
			1);
			
	}

	/* Hud display (image) */
	imgHud(mapInfos, car_player);

	/* Print laps */
	
				
	sprintf(
		txt_msg,
		"%d/%d",
		car_player->my_car.num_lap,
		max_tours);
	
	textOnScreen(
		mapInfos->screen, txt_msg, font, 
		(car_player->my_car.destView.x +
		car_player->my_car.destView.w - 8), 
		(car_player->my_car.destView.y + 7), 
		hud, 2);

	/* Lap time */
	if(car_player->my_car.lap_time
		[car_player->my_car.num_lap] > 0)
	{
		getMilliTime(
			car_player->my_car.lap_time
			[car_player->my_car.num_lap], 
			txt_msg);
		
		textOnScreen(
			mapInfos->screen, 
			txt_msg, font, 
			(car_player->my_car.destView.x +
			car_player->my_car.destView.w - 8), 
			(car_player->my_car.destView.y + 22), 
			hud, 2);
	}

	/* Speed */
	sprintf(txt_msg,"%3.0lf",
		car_player->my_car.speed * 32);
	
	textOnScreen(
		mapInfos->screen, 
		txt_msg, font, 
		(car_player->my_car.destView.x +
		car_player->my_car.destView.w - 30), 
		(car_player->my_car.destView.y +
		car_player->my_car.destView.h - 7), 
		hud, 4);
	
}
		
void textHudComputer(
		struct mapData *mapInfos,
		TTF_Font *font, 
		struct cars *car_player, 
		char *txt_msg, 
		int max_tours, 
		int hud)
{
}

/*void textPlayerHud(
		struct mapData *mapInfos,
		TTF_Font *font, 
		struct cars *car_player, 
		char *txt_msg, 
		int max_tours, 
		int hud)
{

	if(car_player->my_car.computer == 0)
	{
		textHudHuman(mapInfos, 
		font, car_player, txt_msg, max_tours, hud);
	}
	else
	{
		textHudComputer(mapInfos,
		font, car_player, txt_msg, max_tours, hud);
	}

}*/

void show_hud(
	SDL_Surface *screen, 
	TTF_Font *font, 
	struct cars *the_player,
	int pause,
	int max_tours,
	int fps,
	int hud,
	struct mapData *mapInfos)
{
	struct cars *car_player = the_player;
	
	double diff_x, diff_y, vitessep1;
	
	char txt_msg[256];
	
	int current_num_lap;

	SDL_Rect dest;

	
	/*
	sprintf(txt_msg, "FPS: %d", fps);

	textOnScreen(screen, txt_msg, font, 
			(screen->w/2), 
			0,
		       	hud, 5);
	*/
	
	if(pause)
	{
		sprintf(txt_msg,"Pause");
		
		textOnScreen(screen, txt_msg, font, 
				screen->w/2, 
				screen->h/2, 
				hud, 1);
		
		
	}
	else
	{
		while(car_player != NULL)
		{


			/* Position */
			
			if(car_player->my_car.finished == 1)
			{
				sprintf(
					txt_msg,
					"#%d: %s",
					car_player->my_car.position,
					car_player->my_car.player_name);
			
				textOnScreen(
						screen, txt_msg, font, 10, 
						(car_player->my_car.position*20), 
						hud, 0);

			}
	
			

			if(car_player->my_car.computer == 0)
			{
				textHudHuman(
						mapInfos,
						font, 
						car_player, 
						txt_msg, 
						max_tours, 
						hud);
				/*
				sprintf(txt_msg, "%s", car_player->my_car.player_name);

				textOnScreen(
						screen, txt_msg, font, 
						(car_player->my_car.playerNumber*128), 
						screen->h-60, 
						hud, 0);*/
				
				/*if(car_player->my_car.speed > 0.1)
				{
					diff_x = car_player->my_car.car_x - 
						car_player->my_car.last_x;

					diff_y = car_player->my_car.car_y - 
						car_player->my_car.last_y;

					vitessep1 = pow(diff_x, 2) + 
						pow(diff_y,2);
					
					vitessep1 = 50 * sqrt( vitessep1 );
				}
				else
					vitessep1 = 0.0;*/

			
				
				/* Speed */
				
				

				/* Laps */
				
				/*else if (current_num_lap == max_tours)
					sprintf(
						txt_msg,
						"Final lap");
				else
					sprintf(txt_msg, "Finished");
				*/
				

			}
			else
			{
				
				if(car_player->my_car.debugAi == 1)
				{
					sprintf(txt_msg,"%d",car_player->my_car.maxD);
				
					textOnScreen(
							screen, txt_msg, font, 
							0, 0, 
							hud, 0);

					if(car_player->my_car.k.keys[KEY_MOVEUP] == 1)
					{
						sprintf(txt_msg, "UP");
						textOnScreen(
								screen, txt_msg, font,
								70, 20,
								hud, 0
							    );
					}
					if(car_player->my_car.k.keys[KEY_MOVEDOWN] == 1)
					{
						sprintf(txt_msg, "DOWN");
						textOnScreen(
								screen, txt_msg, font,
								70, 35,
								hud, 0
							    );
					}
					if(car_player->my_car.k.keys[KEY_MOVELEFT] == 1)
					{
						sprintf(txt_msg, "LEFT");
						textOnScreen(
								screen, txt_msg, font,
								40, 35,
								hud, 0
							    );
					}
					if(car_player->my_car.k.keys[KEY_MOVERIGHT] == 1)
					{
						sprintf(txt_msg, "RIGHT");
						textOnScreen(
								screen, txt_msg, font,
								10, 35,
								hud, 0
							    );
					}
				
				}
				
			}
			
			car_player = car_player -> next;
		}
	}
}


void drawtext(
	TTF_Font *fonttodraw, SDL_Surface **destsurf, 
	char fgR, char fgG, char fgB, char fgA, 
	char bgR, char bgG, char bgB, char bgA, 
	char text[], int quality)
{
	SDL_Color tmpfontcolor = {fgR,fgG,fgB,fgA};
	SDL_Color tmpfontbgcolor = {bgR, bgG, bgB, bgA};
	if (quality == TSOLID) 
		*destsurf = TTF_RenderText_Solid(fonttodraw, text, 
	  	tmpfontcolor);
	else if (quality == TSHADED) 
		*destsurf = TTF_RenderText_Shaded(fonttodraw, text, 
			tmpfontcolor, tmpfontbgcolor);
	else if (quality == TBLENDED) 
		*destsurf = TTF_RenderText_Blended(fonttodraw, text, 
			tmpfontcolor);
	return;
}
