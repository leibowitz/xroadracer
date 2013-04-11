#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/sge.h>

#include "config.h"

#include "image.h"
#include "main.h"
#include "pixel.h"
#include "editor.h"
#include "time.h"
#include "fill.h"
#include "text.h"
#include "mask.h"
#include "input.h"

#define MAP_CUSTOM_FILE_FORMAT DATA_DIR"/maps/custom/%s"
#define MAP_CUSTOM_CP_FORMAT DATA_DIR"/maps/custom/%s_checkp"
#define MAP_CUSTOM_TXT_FORMAT DATA_DIR"/maps/custom/%s.txt"
#define PATHMINICONSTRUCT DATA_DIR"/construct/road_mini%03d.png"
#define CUSTOMPATH DATA_DIR"/maps/custom/"

int pieceWidth = 40, pieceHeight = 40;


void editorShowPosition(SDL_Surface *screen, 
		SDL_Surface *background, 
		struct coordPt position,
		struct coordPt offset,
		struct constructPieces currentPiece);

void editorSaveCheckp(struct constructPieces *piece, char *name)
{
	FILE *target;
	
	char file[256];

	
	if(piece == NULL)
		return;

	sprintf(file, MAP_CUSTOM_CP_FORMAT, name);

	target = fopen(file,"w");

	if(target == NULL)
		return;

	fprintf(target, "%d %d %d %d\n",	
			piece->x*5+20, piece->y*5+100,
			piece->x*5+180, piece->y*5+100);

	fprintf(target, "%d %d %d %d\n",
			piece->x*5+20, piece->y*5+200,
			piece->x*5+180, piece->y*5+200);

	fclose(target);

	sprintf(file, MAP_CUSTOM_TXT_FORMAT,name);

	target = fopen(file,"w");

	if(target == NULL)
		return;

	fprintf(target, "map_name=%s\n",name);
	fprintf(target, "checkpts=16\n");
	fprintf(target, "player1x=%d\n",piece->x*5+70);
	fprintf(target, "player1y=%d\n",piece->y*5+45);
	fprintf(target, "player1r=90\n");
	fprintf(target, "player2x=%d\n",piece->x*5+130);
	fprintf(target, "player2y=%d\n",piece->y*5+40);
	fprintf(target, "player2r=90\n");

	fclose(target);
}

void editorSaveFile(struct constructPieces *piecesList, char *name)
{
	FILE *target;

	char file[256];

	sprintf(file, MAP_CUSTOM_FILE_FORMAT, name);
	
	target = fopen(file, "w");

	if(target == NULL)
		return;
	
	while(piecesList!= NULL)
	{
		fprintf(target, "%d %d %d\n",
			piecesList->x,
			piecesList->y,
			piecesList->pieceNumber);

		if(piecesList->pieceNumber == 12)
			editorSaveCheckp(piecesList, name);
		
		piecesList = piecesList->next;
	}

	fclose(target);

	printf("Map saved as: %s\n", file);

}

void editorSaveMap(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, 
		struct constructPieces *piecesList)
{
	char map[30]= "";

	if(getKeyboard(background, screen, font, map, 30, "Save as:") > 0)
		editorSaveFile(piecesList, map);
}

void editorShowPieces(SDL_Surface *screen,
		struct coordPt position,
		struct coordPt offset,
		struct constructPieces *piecesList)
{
	SDL_Rect dest;

	/*rimg = SDL_CreateRGBSurface(SDL_SWSURFACE,
			pieceWidth,
			pieceHeight,
			COLOR_DEPTH,
			rmask,
			gmask,
			bmask,
			amask);

	if(rimg == NULL)
		return;
	
	tig = SDL_DisplayFormat(rimg);

	SDL_FreeSurface(rimg);

	if(tig == NULL)
		return;
	
	rimg = tig;*/
	
	while(piecesList != NULL)
	{
		dest.x = piecesList->x - offset.x;
		dest.y = piecesList->y - offset.y;

		/*
		if(dest.x < 0)
			dest.x = 0;
		if(dest.y < 0)
			dest.y = 0;
		*/
		dest.w = piecesList->width;
		dest.h = piecesList->height;

		/*if(dest.x + dest.w >= screen->w)
			dest.w = screen->w - dest.x;*/
				
		if(dest.x >= 0 && dest.y >= 0)
		{
			/*if(piecesList->angRotation == 0)*/
			SDL_BlitSurface(piecesList->img, 
				NULL, screen, &dest);

			/*else
			{
				SDL_FillRect(
					rimg,
					NULL,
					SDL_MapRGB(screen->format,
						0,
						121,
						16) );

				sge_transform(piecesList->img,
					rimg,
					piecesList->angRotation,
					1.0,
					1.0,
					piecesList->img->w/2-1,
					piecesList->img->h/2-1,
					rimg->w/2-1,
					rimg->h/2-1,
					SGE_TSAFE
						);
				if(rimg != NULL)
				{
					SDL_BlitSurface(rimg,
						NULL,screen,&dest);
				}
			}*/
		}
		
		piecesList = piecesList -> next;
	}

	/*SDL_FreeSurface(rimg);*/
}


void editorShowGraduation(SDL_Surface *screen,
		struct coordPt offset,
		struct coordPt maximum)
{
	int ibcl, jbcl;

	if(SDL_MUSTLOCK(screen) )
		SDL_LockSurface(screen);
	
	for(ibcl= 0; ibcl+offset.x < maximum.x; ibcl++)
	{
		putpixel(screen, ibcl, maximum.y, 
			SDL_MapRGB(screen->format, 0, 0, 0) );
	}

	for(jbcl= 0; jbcl+offset.y < maximum.y; jbcl++)
	{
		putpixel(screen, maximum.x, jbcl,
			SDL_MapRGB(screen->format, 0, 0, 0) );
			
	}
	
	
	for(jbcl= 0; jbcl+offset.y < maximum.y && jbcl < screen->h; jbcl+= pieceHeight)
	{
		for(ibcl= 0; ibcl+offset.x < maximum.x && ibcl < screen->w ; ibcl += pieceWidth)
		{
			/*if(ibcl%pieceWidth == 0)*/
			{
				putpixel(screen, ibcl, jbcl,
				SDL_MapRGB(screen->format, 40,40,40));
			}
		}
	}
	/*
	for(ibcl= 0; ibcl < screen->h; ibcl++)
	{
		putpixel(screen, 0, ibcl,
			SDL_MapRGB(screen->format, 0, 0, 0) );
	}
	*/
	if(SDL_MUSTLOCK(screen) )
		SDL_UnlockSurface(screen);

}

void editorAddPiece(struct constructPieces **piecesList,
		struct constructPieces currentPiece)
{
	struct constructPieces *last = NULL, *t = *piecesList;
	
	while(t != NULL)
	{
		last = t;

		if(currentPiece.x == t->x && currentPiece.y == t->y)
		{
			/*printf("remplacement d'une piece ?\n");*/

			t->x = currentPiece.x;
			t->y = currentPiece.y;
			t->img = currentPiece.img;
			t->angRotation = currentPiece.angRotation;
			t->pieceNumber = currentPiece.pieceNumber;

			return;
		}
		
		t = t -> next;
	}

	t = (struct constructPieces*)
		malloc(sizeof(struct constructPieces));

	if(t == NULL)
		return;

	t->next = NULL;
	t->x = currentPiece.x;
	t->y = currentPiece.y;
	/*printf("piece position x %d y %d\n",t->x,t->y);*/
	t->pieceNumber = currentPiece.pieceNumber;
	t->angRotation = currentPiece.angRotation;

	t->img = currentPiece.img;
	
	if(last == NULL)
	{
		*piecesList = t;
	}
	else
	{

		last->next = t;
	}

}

void editorLoadSave(
		char *name,
		struct constructPieces *currentPiece,
		struct constructPieces **piecesList,
		SDL_Surface **availPieces,
		int maxPieces)
{
	FILE *file;
	int x, y, n;

	

	file = fopen(name,"r");

	if(file == NULL)
		return;

	while(fscanf(file,"%d %d %d",&x,&y,&n) == 3)
	{

		currentPiece->x = x;
		currentPiece->y = y;
		currentPiece->pieceNumber = n;
		
		currentPiece->img = availPieces[n];
		
		editorAddPiece(piecesList,
			*currentPiece);
	}
	
	fclose(file);
}

void editorClearList(struct constructPieces **piecesList)
{
	struct constructPieces *tpL = NULL;

	if(piecesList == NULL)
		return;

	tpL = *piecesList;

	while(tpL != NULL)
	{
		*piecesList = tpL->next;
		free(tpL);

		tpL = *piecesList;
	}
}

void editorDeletePiece(struct constructPieces **piecesList,
		struct constructPieces *currentPiece)
{
	struct constructPieces *prev = NULL, *next = NULL, *cur = NULL;

	cur = *piecesList;
	
	while(cur != NULL)
	{
		next = cur->next;
		
		if(cur ->x == currentPiece->x && 
				cur -> y == currentPiece->y)
		{
			free(cur);
			
			cur = NULL;
			
			if(prev != NULL)
				prev->next = next;
			else
				*piecesList = next;
			
			break;
		}
		
		prev = cur;
		
		cur = next;
	}
}

void editorKeyEvent(int key,
		struct coordPt *position,
		struct coordPt *offset,
		SDL_Surface *screen,
		TTF_Font *font,
		SDL_Surface *background,
		struct constructPieces **piecesList,
		SDL_Surface **availPieces,
		int maxPieces,
		struct constructPieces *currentPiece,
		struct coordPt maximum)
{
	SDL_Event tmp_event;
	struct constructPieces saveCurrentPiece;
	char name[250]=CUSTOMPATH;
	
	switch(key)
	{
		case SDLK_PAGEUP:
			position->y -= SCREEN_HEIGHT;
			while(position->y < 0)
				position->y += pieceHeight;
			if(position->y < offset->y)
				offset->y -= SCREEN_HEIGHT;
			while(offset->y < 0)
				offset->y += pieceHeight;
			break;
			
		case SDLK_UP:
			/*if(position->y > 0)*/
			{
			position->y -= pieceHeight;
			if(position->y < 0)
				position->y += pieceHeight;
			if(position->y < offset->y)
				offset->y -= pieceHeight;
			if(offset->y < 0)
			{
				offset->y += pieceHeight;
				/*position->y += pieceHeight;*/
			}
			}
			/*printf("position y %d offset %d\n",
					position->y,
					offset->y);*/
			break;

		case SDLK_LEFT:
			position->x -= pieceWidth;
			if(position->x < 0)
				position->x += pieceWidth;
			if(position->x < offset->x)
				offset->x -= pieceWidth;
			if(offset->x < 0)
			{
				offset->x += pieceWidth;
				/*position->x += pieceWidth;*/
			}
			/*printf("position x %d offset %d\n",
					position->x,
					offset->x);*/
			break;

		case SDLK_PAGEDOWN:
			position->y += SCREEN_HEIGHT;
			while(position->y >= maximum.y - pieceHeight)
				position->y -= pieceHeight;
			if(position->y-offset->y > screen->h - pieceHeight)
				offset->y += SCREEN_HEIGHT;
			while(maximum.y > screen->h && offset->y >= maximum.y-screen->h)
				offset->y -= pieceHeight;
			break;

		case SDLK_DOWN:
			position->y += pieceHeight;
			if(position->y >= maximum.y - pieceHeight)
				position->y -= pieceHeight;
			if(position->y-offset->y > screen->h- 
					pieceHeight)
				offset->y += pieceHeight;
			if(maximum.y > screen->h && 
				offset->y >= maximum.y - screen->h)
			{
				offset->y -= pieceHeight;
				/*position->y -= pieceHeight;*/
			}
			/*printf("position y %d offset %d\n",
					position->y,
					offset->y);*/
			break;

		case SDLK_RIGHT:
			position->x += pieceWidth;
			if(position->x >= maximum.x - pieceWidth)
				position->x -= pieceWidth;
			if(position->x-offset->x > screen->w-
					pieceWidth)
				offset->x += pieceWidth;
			if(maximum.x > screen->w && 
				offset->x >= maximum.x - screen->w)
			{
				offset->x -= pieceWidth;
				/*position->x -= pieceWidth;*/
			}
			/*printf("position x %d offset x %d\n",
					position->x,
					offset->x);*/
			break;

		/*case SDLK_r:
			currentPiece->angRotation += 90;
			while(currentPiece->angRotation >= 360)
				currentPiece->angRotation -= 360;
			break;
		*/
		case SDLK_F2:
			if(--currentPiece->pieceNumber < 0)
				currentPiece->pieceNumber = maxPieces-1;
			currentPiece->img =
				availPieces[currentPiece->pieceNumber];
			currentPiece->width = 
				currentPiece->img->w;
			currentPiece->height = 
				currentPiece->img->h;
			break;

		case SDLK_F3:
			if(++currentPiece->pieceNumber >= maxPieces)
				currentPiece->pieceNumber = 0;

			currentPiece->img = 
				availPieces[currentPiece->pieceNumber];
			currentPiece->width = 
				currentPiece->img->w;
			currentPiece->height = 
				currentPiece->img->h;
			break;

		case SDLK_SPACE:
		case SDLK_RETURN:
			editorAddPiece(piecesList,
					*currentPiece);
			break;

		case SDLK_s:
			editorSaveMap(background, screen, font, *piecesList);
			break;

		case SDLK_l:

			if( getKeyboard(background, screen, font, (name+strlen(CUSTOMPATH)), 30, "Load file:") > 0)
			{
				editorClearList(piecesList);
				saveCurrentPiece = *currentPiece;
				editorLoadSave(
					name,
					currentPiece, 
					piecesList, 
					availPieces, 
					maxPieces);
				*currentPiece = saveCurrentPiece;
			}
			break;

		case SDLK_t:
			/*editorTestMap();*/
			break;

		case SDLK_d:
		case SDLK_DELETE:
			editorDeletePiece(piecesList, currentPiece);
			break;

		case SDLK_ESCAPE:
			tmp_event.type = SDL_QUIT;
			SDL_PushEvent(&tmp_event);
			break;
	}
}

void editorLoop
(SDL_Surface *screen,
 TTF_Font * font,
 SDL_Surface *background,
 SDL_Surface **availPieces,
 int maxPieces
)
{
	/* ---------------------------------- */
	int loop = 1;
	SDL_Event event;
	struct constructPieces *piecesList = NULL, 
			       currentPiece;
	
	struct coordPt position, offset, maximum;

	SDL_Rect dest;

	/* ---------------------------------- */
	
	maximum.x = MAX_MAP_WIDTH;
	maximum.y = MAX_MAP_HEIGHT;

	
	offset.x = 0;
	offset.y = 0;

	position.x = 0;
	position.y = 0;


	if(availPieces == NULL)
		return;
	

	currentPiece.next = NULL;
	currentPiece.x = position.x;
	currentPiece.y = position.y;
	currentPiece.pieceNumber = 0;
	currentPiece.angRotation = 0;

	currentPiece.img = availPieces[0];
	currentPiece.width = currentPiece.img->w;
	currentPiece.height = currentPiece.img->h;

	while(loop)
	{
		SDL_BlitSurface(background,NULL,screen,NULL);

		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
				editorKeyEvent(event.key.keysym.sym,
						&position,
						&offset,
						screen,
						font,
						background,
						&piecesList,
						availPieces,
						maxPieces,
						&currentPiece,
						maximum);
				break;

				case SDL_KEYUP:
					break;

				case SDL_QUIT:
					loop = 0;
					break;
			}
		}

		dest.x = position.x - offset.x;
		dest.y = position.y - offset.y;

		currentPiece.x = position.x;
		currentPiece.y = position.y;

		
		editorShowGraduation(
				screen,
				offset,
				maximum);
			
		editorShowPieces(
			screen,
			position,
			offset,
			piecesList);
		
		SDL_BlitSurface(currentPiece.img, NULL, screen, &dest);
		
		editorShowPosition(
				screen, 
				background, 
				position, 
				offset,
				currentPiece);
		
		
		SDL_Flip(screen);
		
		SDL_Delay( time_left() );
		
	}

	editorClearList(&piecesList);

	/*while(piecesList != NULL)
	{
		tpL = piecesList;
		
		piecesList = piecesList -> next;

		free(tpL);
	}*/
	
	
}

void editorShowPosition(SDL_Surface *screen, 
		SDL_Surface *background,
		struct coordPt position,
		struct coordPt offset,
		struct constructPieces currentPiece)
{
	Uint32 color;

	int ibcl, jbcl;

	color = SDL_MapRGB(screen->format,
			255,255,255);

	if(SDL_MUSTLOCK(screen) )
		SDL_LockSurface(screen);
	
	/* top/bottom border */
	for(ibcl = position.x-offset.x; 
			ibcl < position.x-offset.x+currentPiece.width; ibcl++)
	{
		putpixel(screen, 
				ibcl, 
				position.y-offset.y, 
				color);
		putpixel(screen, 
				ibcl, 
				position.y-offset.y+currentPiece.height-1, 
				color);
	}

	/* left/right border */
	for(jbcl = position.y-offset.y; 
			jbcl < position.y-offset.y+currentPiece.height; jbcl++)
	{
		putpixel(screen, 
				position.x-offset.x, 
				jbcl, 
				color);
		
		putpixel(screen, 
				position.x-offset.x+currentPiece.width-1, 
				jbcl, 
				color);
	}

	if(SDL_MUSTLOCK(screen) )
		SDL_UnlockSurface(screen);
}

void startNewEditor(
		SDL_Surface *screen,
		TTF_Font *font,
		SDL_Joystick *joy/*,
		int argc, 
		char **argv*/
		
	      )
{
	/* ----------------------------- */
	char message[256];
	
	SDL_Surface *background = NULL, *bgTemp=NULL;

	SDL_Surface **availPieces = NULL;
	
	int canvas_width = SCREEN_WIDTH,
	    canvas_height = SCREEN_HEIGHT,
	    maxPieces = CONSTRUCT_MAX_PIECES, bcl;
	/* ----------------------------- */
	
	availPieces = (SDL_Surface**) 
		malloc(sizeof(SDL_Surface)*maxPieces);
	
	if(availPieces == NULL)
		return;

	
	
	fillSurfaceWithBlack(screen);
	
	/* Show the status on screen */
	sprintf(message,"Loading...");
	
	print_hud_text(screen, message, 1, 1, font, 1);
	
	SDL_UpdateRect(screen, 0, 0, 100, 20);
	/* Load components */
	
	for(bcl=0; bcl < maxPieces; bcl++)
	{
		snprintf(message, 250,
		PATHMINICONSTRUCT,(bcl+1));
		
		availPieces[bcl] = surface_from_img(message);
	}

	/* Prepare the editor window with */
	
	background = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			canvas_width,
			canvas_height,
			COLOR_DEPTH,
			rmask,
			gmask,
			bmask,
			amask);
	
	if(background == NULL)
	{
		fprintf(stderr, "CreateRGBSurface failed: %s\n",
				SDL_GetError());
	}
	else
	{
		bgTemp = SDL_DisplayFormat(background);
		
		SDL_FreeSurface(background);
		background = bgTemp;

		if(bgTemp != NULL)
		{
		/*printf("back color: %d\n",
			SDL_MapRGB(screen->format, 0, 121, 16));*/
		
		if(SDL_FillRect(bgTemp, NULL, 
			SDL_MapRGB(screen->format, 0, 121, 16)) ==0)
			{
				SDL_BlitSurface(
						bgTemp,
						NULL, 
						screen,
						NULL);
				/*SDL_Flip(screen);
				printf("updating\n");*/
			}
		}

	}
	
	/*if(piece1 != NULL)
	{
		SDL_BlitSurface(piece1, NULL, screen,NULL);
	}*/
	/* Refresh the screen */
	/*SDL_UpdateRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);*/
	/*SDL_Delay(1000);*/
	SDL_Flip(screen);
	
	if(availPieces != NULL && background != NULL)
	{
		editorLoop(screen,font, background,availPieces,maxPieces);
		
	}
	/* Free all loaded components */
	if(availPieces != NULL)
		free(availPieces);
	if(background != NULL)
		SDL_FreeSurface(background);
	/* exit */
}
