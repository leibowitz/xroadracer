#include <math.h>
#include <SDL/SDL.h>


#include "struct_car.h"
#include "map_struct.h"
#include "pixel.h"
#include "angular.h"
#include "key.h"

#include "collision.h"



int computerSight(struct car_properties *p,
		int start, int stop, float r,
		struct mapData *mapInfos,
		SDL_Surface *target,
		SDL_Surface *source,
		int x, int y,
		Uint32 baseColor)
{
	int bcl, fx, fy, distance = -1;

	float cos, sin, ang;

	Uint32 gcol;
	
	ang = r/ARAD;
	
	cos = cosf(ang);

	sin = sinf(ang);
	
	for(bcl = start; bcl < stop; bcl++)
	{
		fx = x+roundf(bcl*cos);
		fy = y+roundf(bcl*sin);

		if(target != NULL && p->debugAi == 1)
			putpixel(target, 
				fx, 
				fy,
				baseColor);

		if(source != NULL)
		{
			gcol = getpixel(source,
				fx,
				fy);

			if(gcol != baseColor)
			{
				break;
			}
			else
				distance = bcl;
		}
		
	}
	
	return(distance);
	
}

float MAX(float a, float b)
{
	if(a > b)
		return(a);
	else
		return(b);
}

float MIN(float a, float b)
{
	if(a < b)
		return(a);
	else
		return(b);
}

float fuzzyGrade(int x, int x0, int x1)
{
	float result = 0;

	if(x <= x0)
		result = 0;
	else if(x >= x1)
		result = 1;
	else
		result = (x/(float)(x1-x0)) - 
			(x0/(float)(x1-x0));

	return(result);
}

float fuzzyReverseGrade(int x, int x0, int x1)
{
	float result = 0;

	if(x <= x0)
		result = 1;
	else if(x >= x1)
		result = 0;
	else
		result = (-x/(float)(x1-x0)) + 
			(x1/(float)(x1-x0));

	return(result);
}

struct s_dist 
{
	float near;
	float far;
};

struct s_dist getFuzzy(int d)
{
	struct s_dist dist;

	dist.near = fuzzyReverseGrade(d, 30, 70);

	dist.far = fuzzyGrade(d, 100, 150);

	return(dist);
}

/* -------------------------|
 * Intelligence Artificielle| 
 * -------------------------|
 *
 * computerView() 
 *  - int x, int y : coordonnées de position de la voiture 
 *  sur le circuit. Ca sert de point de départ de "vision"
 *
 *  - float r : rotation angulaire de la voiture. 
 *  Attention le sens est inversé par rapport au 
 *  cercle trigonométrique (sens horaire).
 *
 *  - int start, int stop : respectivement le point de 
 *  départ et le point de fin de la recherche.
 *  Exemple: start=10 et stop=50 signifie que 
 *  l'on doit vérifier de 10 pixels vers l'avant jusqu'à 
 *  50 pixels vers l'avant.
 *
 *  - strut mapData *mapInfos : structure contenant pas 
 *  mal d'informations et de pointeurs vers les zones 
 *  mémoires allouées à l'affiche notemment.
 *  
 *  - struct car_properties *p : structure contenant 
 *  les informations relatives à la voiture, et au 
 *  joueur. Elle est utilisée pour changer l'état 
 *  des touches (avant, gauche, droite, frein) 
 *  du computer et de cette manière modifier sa 
 *  position.
 *
 *  
 * */
void computerView(int x, int y, float r,
		int start, int stop,
		struct mapData *mapInfos,
		struct car_properties *p)
{
	
	int distance = 0, left, right;

	int bcl, d, max = 0, maxSide = 0;

	float maxr, cosr, sinr, cost, sint;
	
	Uint32 baseColor = 0, black, red, white;
	
	struct s_dist dist, forward;

	/* Initialisation de quelques couleurs de base 
	 * utiles pour les tests */
	
	black = SDL_MapRGB(mapInfos->screen->format,
		0,0,0);

	red =  SDL_MapRGB(mapInfos->screen->format,
		255,0,0);

	white = SDL_MapRGB(mapInfos->screen->format,
			255,255,255);

	/* On récupere la couleur sur laquelle le 
	 * computer est positionné pour l'instant. 
	 *
	 * Note: dans tous les algorithmes qui vont 
	 * suivre, il n'y a que deux couleurs de base :
	 * le blanc, qui signifie "route", et 
	 * le noir qui signifie "hors-piste".
	 * */
	baseColor = getpixel(mapInfos->mpRoad,
			x, y);

	/* Comme dit précedemment, il faut utiliser 
	 * l'état des touches du joueurs courant 
	 * pour faire déplacer la voiture. 
	 *
	 * Une valeur 0 signifie "non pressé", 
	 * tandis que 1 signifie "touche pressé" */
	
	p->k.keys[KEY_MOVELEFT] = 0;
	p->k.keys[KEY_MOVERIGHT] = 0;
	p->k.keys[KEY_MOVEUP] = 0;
	p->k.keys[KEY_MOVEDOWN] = 0;
	

	/* Le comportement qui va suivre dépend 
	 * de l'endroit où se trouve le computer,
	 * c'est-à-dire "route" ou "hors-piste"*/
	if(baseColor == black)
	{
		/* Dans ce cas, on recherche la distance 
		 * minimum qui sépare la voiture de la route */
		max = stop;
	}
	else
	{
		/* Dans l'autre, il faut trouver la 
		 * distance maximum de route disponible 
		 * vers laquelle se diriger. */
		max = 0;
	}
	
	

	/* Cette boucle va faire un scan avancé 
	 * de toutes les directions, partant de 
	 * 60 degrés vers la gauche, jusqu'à 
	 * 60 degrés vers la droite. 
	 *
	 * En prenant toujours le point de départ 
	 * "start" et le point de fin "stop".
	 *
	 * Pour activer l'affichage des axes qui 
	 * sont scannés, il faut appuyer sur "F1" 
	 * pendant une partie. 
	 *
	 * Les directions scannées seront alors 
	 * affichées à l'écran.
	 * */
	for(bcl = -60; bcl <= 60; bcl+= 20)
	{
		/* Récuperer la distance maximum ou minimum */
		d = computerSight(p, start, stop, r+bcl, mapInfos, 
			mapInfos->mpDecoration,
			mapInfos->mpRoad,
			x, y, 
			baseColor);

		/* Sauvegarder la valeur de la distance 
		 * qui sépare la voiture du bord de la route 
		 * ou de la route dans la direction tout droit. */
		if(bcl == 0)
			distance = d;

		/* C'est ici que l'algorithme diffère en 
		 * fonction de la position de la voiture ;
		 * encore une fois c'est-à-dire si elle est 
		 * sur la route ou sur l'extérieur de la piste.*/

		if(baseColor != black)
		{
			/* Si on se trouve sur la route, 
			 * alors il faut trouver l'axe 
			 * de rotation pour lequel la 
			 * distance de piste disponible 
			 * est maximum. 
			 *
			 * C'est-à-dire l'endroit où 
			 * se diriger. */
			if(d > max)
			{
				max = d;
				maxr = r+bcl;
				maxSide = bcl;
			}
		}
		else
		{
			/* Si la voiture se trouve sur le 
			 * côté de la piste, alors il faut 
			 * plutôt rechercher l'angle 
			 * de l'axe qui sépare la voiture 
			 * de la route sur une distance 
			 * minimum. */
			if(d < max)
			{
				max = d;
				maxr = r+bcl;
				maxSide = bcl;
			}
		}
		
	}

	/* Informations récoltées pour le debug visuel
	 *
	 * p->maxD Correspond à la distance maximum 
	 * ou minimum du scan complet (120 degrés 
	 * par pas de 20 degrés ; centré sur l'axe 
	 * de rotation de la voiture) */
	p->maxD = max;

	/* Basic implementation of Fuzzy Logic */ 
	dist = getFuzzy(max);

	forward = getFuzzy(distance);

	/*printf("far %.1lf - near %.1lf\n",
		dist.far, dist.near);*/
	
	
		
	/* ------------------------------- 
	 * La recolte des informations est 
	 * faites, maintenant il faut passer 
	 * aux décisions à prendre. 
	 ------------------------------- */


	
	/* Si l'angle vers lequel il faut se 
	 * diriger est différent de l'axe de 
	 * rotation actuel de la voiture, 
	 * cela signifie qu'il faut tourner. */
	if( maxSide != 0 )
	{
		
		if(maxSide > 0 )
			p->k.keys[KEY_MOVERIGHT] = 1;
		else
			p->k.keys[KEY_MOVELEFT] = 1;

		/* si la voiture se trouve sur la route*/		
		if(baseColor != black)
		{

			/* ceci permet de voir si la voiture 
			 * se dirige trop près d'un bord. 
			 *
			 * Dans ce cas, il vaut mieux freiner.*/
			/*if(max < stop/2)
				p->k.keys[KEY_MOVEDOWN] = 1;*/
		}
		/* Si la voiture est en "hors-piste", 
		 * autant accélerer. */
		else
			p->k.keys[KEY_MOVEUP] = 1;

	}
	/* Par contre si la distance la plus grande ou la plus 
	 * petite scanné correspond à l'axe de rotation de la 
	 * voiture, alors il ne faut pas tourner. */
	else
	
	{
		/* Si la voiture est sur la route */
		if(baseColor != black)
		{
	 		/* Dans le cas où la distance vers l'avant 
			 * est réduite, alors c'est que le computer 
			 * se dirige vers l'extérieur, ce qui est 
			 * embettant. */
			if(distance < stop/10 && p->speed > 20)
			{
				p->k.keys[KEY_MOVEDOWN] = 1;
			}
			else
				p->k.keys[KEY_MOVEUP] = 1;
		}
		else
		/* Si la voiture fait du "hors-piste", il vaudrait 
		 * mieux la rediriger vers la route. 
		 * Mais c'est déjà le cas, alors accélerons !! */
		{
			p->k.keys[KEY_MOVEUP] = 1;
		}
	}

	/* New algorithm from Fuzzy theory */	
	/*
	p->k.keys[KEY_MOVEUP] = 0;
	p->k.keys[KEY_MOVEDOWN] = 0;
	
	if(forward.near > 0.2)
		p->k.keys[KEY_MOVEDOWN] = 1;
	else if(abs(maxSide) < 5)
		p->k.keys[KEY_MOVEUP] = 1;

	*/


}

/* ------------------------------------------------------ */

void computerIa(struct car_properties *p, struct mapData *mapInfos)
{
	Uint32 color;

	int x, y;
	
	if(mapInfos->mpRoad != NULL && p != NULL)

	{
	
		if(mapInfos->mpDecoration != NULL)
		{
			color = SDL_MapRGB(
					mapInfos->mpDecoration->format,
				0,0,255);

			SDL_FillRect(mapInfos->mpDecoration, NULL,
					color);
		
			if(SDL_MUSTLOCK(mapInfos->mpDecoration))
				SDL_LockSurface(mapInfos->mpDecoration);
		}
		if(SDL_MUSTLOCK(mapInfos->mpRoad))
			SDL_LockSurface(mapInfos->mpRoad);

		x = (int)roundf(p->car_x);
		y = (int)roundf(p->car_y);


		computerView(
			x, 
			y,
				p->rotation,
				0, 200,
				mapInfos,
				p);

		
		if(SDL_MUSTLOCK(mapInfos->mpRoad))
			SDL_LockSurface(mapInfos->mpRoad);

		if(mapInfos->mpDecoration != NULL)
		{
			if(SDL_MUSTLOCK(mapInfos->mpDecoration))
				SDL_LockSurface(mapInfos->mpDecoration);
		}
		

	}
}
