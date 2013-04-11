#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL/SDL.h>

#include "main.h"

/* Little function, calculating time left for the INTERVAL */
unsigned int time_left(void)
{
	static unsigned int next_time = 0;
	unsigned int now;

	now = SDL_GetTicks();

	if(next_time <= now)
	{
		next_time = now+INTERVAL;
		return 0;
	}
	return (next_time-now);
}

void get_minute_second(char **text_second, int time)
{
	int minutes,second,milli, tmp;
	char my_text[256] = "\0";
	
	if(text_second == NULL)
		return;
	
	tmp = abs(time)/100;
	minutes = (tmp/60);
	second  = (tmp)%60;
	milli = abs(time)%100;
	if(time > 0)
		strcpy(*text_second,"+");
	else
		strcpy(*text_second,"-");
	if(minutes > 0)
		sprintf(my_text,"%2d min ",minutes);
	strcat(*text_second,my_text);
	sprintf(my_text,"%2d.%02d sec",second,milli);
	strcat(*text_second,my_text);
	return;
}


