#include <stdio.h>
#include <SDL/SDL.h>

#include "struct_car.h"
#include "key.h"
#include "car_move.h"
#include "map_struct.h"

/* Print modifier info */
void PrintModifiers( SDLMod mod )
{
	/*printf( "Modifers: " );
	*/
	/* If there are none then say so and return */
	if( mod == KMOD_NONE )
	{
	        /*printf( "None\n" );*/
	        return;
	}

	/* Check for the presence of each SDLMod value */
	/* This looks messy, but there really isn't    */
	/* a clearer way.                              */
	/*if( mod & KMOD_NUM ) printf( "NUMLOCK " );
	if( mod & KMOD_CAPS ) printf( "CAPSLOCK " );
	if( mod & KMOD_LCTRL ) printf( "LCTRL " );
	if( mod & KMOD_RCTRL ) printf( "RCTRL " );
	if( mod & KMOD_RSHIFT ) printf( "RSHIFT " );
	if( mod & KMOD_LSHIFT ) printf( "LSHIFT " );
	if( mod & KMOD_RALT ) printf( "RALT " );
	if( mod & KMOD_LALT ) printf( "LALT " );
	if( mod & KMOD_CTRL ) printf( "CTRL " );
	if( mod & KMOD_SHIFT ) printf( "SHIFT " );
	if( mod & KMOD_ALT ) printf( "ALT " );
	printf( "\n" );*/
}
    
/* Print all information about a key event */
void PrintKeyInfo( SDL_KeyboardEvent *key )
{
	/* Is it a release or a press? */
	/*printf("Keyboard key ");
	if( key->type == SDL_KEYUP )
	        printf( "Release: -" );
	else
	        printf( "Press:   -" );
	*/
	/* Print the hardware scancode first */
	/*printf( "Scancode: 0x%02X", key->keysym.scancode );*/
	/* Print the name of the key */
	/*printf( ", Name: %s", SDL_GetKeyName( key->keysym.sym ) );*/
	/* We want to print the unicode info, but we need to make */
	/* sure its a press event first (remember, release events */
	/* don't have unicode info                                */
	if( key->type == SDL_KEYDOWN )
	{
	        /* If the Unicode value is less than 0x80 then the    */
	        /* unicode value can be used to get a printable       */
	        /* representation of the key, using (char)unicode.    */
	        /*printf(", Unicode: " );
	        if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
	        printf( "%c (0x%04X)", (char)key->keysym.unicode,
	                key->keysym.unicode );
	        }
	        else{
	        printf( "? (0x%04X)", key->keysym.unicode );
	        }*/
	}
	/*printf( "\n" );*/
	/* Print modifier info */
	PrintModifiers( key->keysym.mod );
	
	return;
}

void keyCheckCommand(struct car_properties *c, int frameTime)
{
	Uint8 *keys;

	struct playerKeys k = c->k;

	int turn = 0, accelBrake = 0;
	
	if(c->computer)
	{
		if( k.keys[KEY_MOVEUP] == 1 )
			accelBrake++;

		if( k.keys[KEY_MOVEDOWN] == 1 )
			accelBrake--;

		if( k.keys[KEY_MOVELEFT] == 1 )
			turn--;

		if( k.keys[KEY_MOVERIGHT] == 1 )
			turn++;

	}
	else
	{
		keys = SDL_GetKeyState(NULL);
	
	
		if(keys[ k.keys[KEY_MOVEUP] ])
			accelBrake++;

		if(keys[ k.keys[KEY_MOVEDOWN] ])
			accelBrake--;
			
		if(keys[ k.keys[KEY_MOVELEFT] ])
			turn--;

		if(keys[ k.keys[KEY_MOVERIGHT] ])
			turn++;

	}	
	
	if(accelBrake>0)
		car_move_fw(c, frameTime);
	else if(accelBrake < 0)
		car_move_bk(c, frameTime);
	else
		forward_noaccel(c, frameTime);

	if(turn!=0)
		car_turn(c, frameTime, turn);
	
}

void key_handling_press(
	SDL_Event *p_event, 
	int **p_key_fct_tab)
{
	SDL_Event event;
	
	int *key_fct_tab;
	
	if(p_event == NULL || p_key_fct_tab == NULL)
		return;
	
	if(*p_key_fct_tab == NULL)
		return;
	
	event = *p_event;
	
	key_fct_tab = *p_key_fct_tab;
	
	
	if( event.key.keysym.sym  ==  key_fct_tab[1] )
	{
		
	}
}

void key_read_config(int *p_key_fct_tab[], int *p_key_fct_sdlcode[])
{
	int *key_fct_tab, *key_fct_sdlcode;
	
	if(p_key_fct_tab == NULL)
	return;
	if(p_key_fct_sdlcode == NULL)
	return;
	
	key_fct_tab = *p_key_fct_tab;
	
	key_fct_sdlcode = *p_key_fct_sdlcode;
	
	if(key_fct_tab == NULL)
	return;
	if(key_fct_sdlcode == NULL)
	return;
	
	
}

void keyPause(Uint32 maxTime)
{
	int loop = 1;

	Uint32 time, endTime;
	
	
	SDL_Event event;

	Uint8 *keys;
	
	time = SDL_GetTicks();
	
	endTime = time + maxTime;
	
	while(loop == 1)
	{
		
		keys = SDL_GetKeyState(NULL);
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
				loop = 0;
				break;
			}
		}
		
		time = SDL_GetTicks();

		if(time > endTime)
			loop = 0;
	}
	

}

void switchAiDebug(struct cars *p, int n)
{
	int bcl = 1;
	
	while(p != NULL)
	{
		if(bcl == n)
			p->my_car.debugAi = !p->my_car.debugAi;

		p = p->next;
	}
}

int keyGetEvents( 
	int game_started, 
	int *pause, 
	int *debug_ia,
	int *hud,
	struct mapData *mapInfos)
{
	SDL_Event event, tmp_event;

	/*int key_print_code;*/
	
	{
		/****************************************
		 Handle Key Press and Release 
		****************************************/
		while(SDL_PollEvent(&event))
		{  
			switch(event.type)
			{  
				/* We are only worried about SDL_KEYDOWN 
				and SDL_KEYUP events */
				
				/* --- Here is keypress --- */
				case SDL_KEYDOWN:
				/* Check the SDLKey values and move 
				change the coords */
				
				/*key_handling_press(&event, 
					&key_fct_tab);*/
					
				switch( event.key.keysym.sym )
				{
				        
				        /* Bad way of doing the switch to fs 
				        case SDLK_f:
					SDL_WM_ToggleFullScreen(surface);
				        break;
				        */
				        /*case SDLK_KP7:
				        color_r +=1;
				        break;
				        
				        case SDLK_KP1:
				        color_r -=1;
				        break;
				        
				        case SDLK_KP8:
				        color_g +=1;
				        break;
				        
				        case SDLK_KP2:
				        color_g -=1;
				        break;
				        
				        case SDLK_KP9:
				        color_b +=1;
				        break;
				        
				        case SDLK_KP3:
				        color_b -=1;
				        break;*/
				        
				        case SDLK_ESCAPE:
				        /*if(show_game_menu(surface,background,font) == 1 )*/
				        {
				        	tmp_event.type = SDL_QUIT;
				        	SDL_PushEvent(&tmp_event);
				        }
				        break;
				        
				        case SDLK_p:
					if(pause != NULL)
				        *pause = !*pause;
				        break;
				        
				        /*case SDLK_r:
				        rotate = !rotate;
				        break;*/
				        
				        case SDLK_a:
					if(debug_ia != NULL)
				        *debug_ia = !*debug_ia;
				        /*
				        computer = !computer;
				        object_xvel = 0;
				        object_yvel = 0;*/
				        
				        /*car_player->my_car.computer = 
				        	!car_player->my_car.computer;
				        car_player->my_car.xvel = 0;
				        car_player->my_car.yvel = 0;*/
				        break;

					case SDLK_h:
					if(hud != NULL)
					{
						(*hud)++;
						if(*hud > 2)
							*hud = 0;
					}
					break;
				        
				        /*case SDLK_c:
				        clear_screen(surface,
				        SDL_MapRGB(surface->format, 0, 0, 0));
				        break;

				        case SDLK_d:
				        draw_active = !draw_active;
				        break;*/

					case SDLK_F1:
					switchAiDebug(mapInfos->p, 1);
					break;
				        
				        case SDLK_F2:
					switchAiDebug(mapInfos->p, 2);
				        /*draw_sprite1(surface,object_x,object_y,
				        	variance_color_r,
				        	variance_color_g,
				        	variance_color_b,
				        	0);*/
				        break;
				        
				        case SDLK_F3:
				        /*draw_sprite1(surface,object_x,object_y,
				        	variance_color_r,
				        	variance_color_g,
				        	variance_color_b,
				        	1);*/
				        break;
				        
				        default:
				        break;
				}
				/*PrintKeyInfo( &event.key );*/
				break;
		
				
				case SDL_QUIT:
				/* Set whatever flags are necessary to */
				/* end the main game loop here */
				return(0);
				break;
				/*
				if( (pause != NULL && *pause == 1) || game_started == 0)
					continue;*/
				
				/*
				case SDL_JOYBUTTONDOWN:  
				printf("joystick button %d pressed\n",
							event.jbutton.button);
				if ( event.jbutton.button == 0 ) 
				{
				}
				break;
				*/
				/*	
				while(car_player != NULL)
				{
					if(car_player->my_car.computer == 0)
					{
						case SDL_KEYUP:
						key_print_code = event.key.keysym.sym;
						
						
						if( key_print_code  ==  key_fct_sdlcode[KEY_MOVEUP] )
						{
							if( car_player->my_car.yvel < 0 )
								car_player->my_car.yvel = 0;
						}
						else if(key_print_code == key_fct_sdlcode[KEY_MOVERIGHT])
						{
							if( car_player->my_car.xvel > 0 )
								car_player->my_car.xvel = 0;
						}
						else if(key_print_code == key_fct_sdlcode[KEY_MOVEDOWN]) 
						{
							if( car_player->my_car.yvel > 0 )
								car_player->my_car.yvel = 0;
						}
						else if(key_print_code == key_fct_sdlcode[KEY_MOVELEFT]) 
						{
							if( car_player->my_car.xvel < 0 )
							       car_player->my_car.xvel = 0;
						      
						}

						break;
					}
					car_player = car_player -> next;
				}*/
			}
		}
	}

	return(1);
}

