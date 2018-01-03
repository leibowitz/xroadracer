#include <math.h>
#include <SDL/SDL.h>
#include "fann.h"


#include "struct_car.h"
#include "map_struct.h"
#include "pixel.h"
#include "angular.h"
#include "computer.h"
#include "key.h"

#include "collision.h"


struct ai_data* load_ai_data() {
 struct ai_data *data = (struct ai_data*)malloc(sizeof(struct ai_data));
 data->road_ann_fw = fann_create_from_file("../training/map1_road_fw.net");
 data->road_ann_bw = fann_create_from_file("../training/map1_road_bw.net");
 data->road_ann_right = fann_create_from_file("../training/map1_road_right.net");
 data->road_ann_left = fann_create_from_file("../training/map1_road_left.net");
 
 data->offroad_ann_fw = fann_create_from_file("../training/map1_offroad_fw.net");
 data->offroad_ann_bw = fann_create_from_file("../training/map1_offroad_bw.net");
 data->offroad_ann_right = fann_create_from_file("../training/map1_offroad_right.net");
 data->offroad_ann_left = fann_create_from_file("../training/map1_offroad_left.net");
 return data;
}

void clear_ai_data(struct ai_data* data) {
 fann_destroy(data->road_ann_fw);
 fann_destroy(data->road_ann_bw);
 fann_destroy(data->road_ann_right);
 fann_destroy(data->road_ann_left);
 
 fann_destroy(data->offroad_ann_fw);
 fann_destroy(data->offroad_ann_bw);
 fann_destroy(data->offroad_ann_right);
 fann_destroy(data->offroad_ann_left);
}


int computerSight(struct car_properties *p,
		int start, int stop, float r,
		struct mapData *mapInfos,
		SDL_Surface *target,
		SDL_Surface *source,
		int x, int y,
		Uint32 baseColor,
		Uint32 black)
{
	int bcl, fx, fy, distance = -1;
        Uint32 color = SDL_MapRGB(mapInfos->screen->format, 200,150,100);

	float cos, sin, ang;

	Uint32 gcol;
	
	ang = r/ARAD;
	
	cos = cosf(ang);

	sin = sinf(ang);
	
	for(bcl = start; bcl < stop; bcl++)
	{
		fx = x+roundf(bcl*cos);
		fy = y+roundf(bcl*sin);

                if (baseColor == black && (
                        fx > mapInfos->mpBg->w ||
                        fy > mapInfos->mpBg->h ||
                        fx < 0 ||
                        fy < 0)
                ) {
                        distance = stop-1;
                        break;
                }

		if(target != NULL && p->debugAi == 1) {
			putpixel(target, 
				fx, 
				fy,
				color);
                }

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

void print_output(int *keys) {
        printf("%d %d %d %d\n\n", 
                keys[KEY_MOVEUP],
                keys[KEY_MOVEDOWN],
                keys[KEY_MOVERIGHT],
                keys[KEY_MOVELEFT]
                );
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
int computerView(int x, int y, float r,
		int start, int stop,
		struct mapData *mapInfos,
		struct car_properties *p)
{
	
	int distance = 0, left, right;

	int bcl, maxSide = 0;
        int d, max = 0;
        int i = 0;
        int ret = 0;
        float input[18];

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

        if (baseColor == black) {
                ret = 1;
        } else {
                ret = 2;
        }

	/* Comme dit précedemment, il faut utiliser 
	 * l'état des touches du joueurs courant 
	 * pour faire déplacer la voiture. 
	 *
	 * Une valeur 0 signifie "non pressé", 
	 * tandis que 1 signifie "touche pressé" */
	
        if (p->computer == 1) {
	p->k.keys[KEY_MOVELEFT] = 0;
	p->k.keys[KEY_MOVERIGHT] = 0;
	p->k.keys[KEY_MOVEUP] = 0;
	p->k.keys[KEY_MOVEDOWN] = 0;
        }
	

        input[i++] = roundf(p->speed);
	/* Le comportement qui va suivre dépend 
	 * de l'endroit où se trouve le computer,
	 * c'est-à-dire "route" ou "hors-piste"*/
	if(baseColor == black)
	{
                if (p->trainAi == 1) {
                        printf("%d ", (int)roundf(p->speed));
                }
		/* Dans ce cas, on recherche la distance 
		 * minimum qui sépare la voiture de la route */
		max = stop;
                /*input[i++] = 0;
                if (p->trainAi == 1) {
                        printf("0 ");
                }*/
	}
	else
	{
                if (p->trainAi == 2) {
                        printf("%d ", (int)roundf(p->speed));
                }
		/* Dans l'autre, il faut trouver la 
		 * distance maximum de route disponible 
		 * vers laquelle se diriger. */
		max = 0;
                /*input[i++] = 1;
                if (p->trainAi == 1) {
                        printf("1 ");
                }*/
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
	for(bcl = -160; bcl <= 160; bcl+= 20)
	{
		/* Récuperer la distance maximum ou minimum */
		d = computerSight(p, start, stop, r+bcl, mapInfos, 
			mapInfos->mpDecoration,
			mapInfos->mpRoad,
			x, y, 
			baseColor,
                        black);
                input[i++] = (stop-d)/20;
                if (baseColor == black) {
                        if (p->trainAi == 1) {
                                printf("%d ", (stop-d)/20);
                        }
                } else {
                        if (p->trainAi == 2) {
                                printf("%d ", (stop-d)/20);
                        }
                }

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

        if (baseColor == black) {
                if (p->trainAi == 1) {
                        printf("\n\n");
                }
        } else {
                if (p->trainAi == 2) {
                        printf("\n\n");
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
        if (p->computer == 1) {
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

                if (baseColor == black) {
                        if(p->trainAi == 1) {
                                print_output(p->k.keys);
                        }
                } else {
                        if(p->trainAi == 2) {
                                print_output(p->k.keys);
                        }
                }

                if (mapInfos->drivingdata->offroad_ann_fw == NULL) {
                        return ret;
                }

                 float *output_fw, *output_bw, *output_right, *output_left;
                 if (black == baseColor) {
                         output_fw = fann_run(mapInfos->drivingdata->offroad_ann_fw, input);
                         output_bw = fann_run(mapInfos->drivingdata->offroad_ann_bw, input);
                         output_right = fann_run(mapInfos->drivingdata->offroad_ann_right, input);
                         output_left = fann_run(mapInfos->drivingdata->offroad_ann_left, input);
                 } else {
                         output_fw = fann_run(mapInfos->drivingdata->road_ann_fw, input);
                         output_bw = fann_run(mapInfos->drivingdata->road_ann_bw, input);
                         output_right = fann_run(mapInfos->drivingdata->road_ann_right, input);
                         output_left = fann_run(mapInfos->drivingdata->road_ann_left, input);
                 }

                 if (output_fw == NULL || output_bw == NULL || output_right == NULL || output_left == NULL) {
                         return ret;
                 }

                 int moveup = 0, movedown = 0, moveright = 0, moveleft = 0;
                 
                 moveup = (int)roundf(*output_fw);
                 movedown = (int)roundf(*output_bw);

                 if (*output_fw > *output_bw) {
                         movedown = 0;
                 } else {
                         moveup = 0;
                 }

                 moveright = (int)roundf(*output_right);
                 moveleft = (int)roundf(*output_left);
 
                 if (*output_right > *output_left) {
                         moveleft = 0;
                 } else if (*output_left > *output_right) {
                         moveright = 0;
                 }

                if (moveright == 1 && moveleft == 1) {
                        moveright = 0;
                        moveleft = 0;
                } else if (moveright == 0 && moveleft == 0) {
                        if (*output_right > 0.3) {
                                moveright = 1;
                        } else if (*output_left > 0.3) {
                                moveleft = 1;
                        }
                }
                
                if (moveup == 1 && movedown == 1) {
                        moveup = 0;
                }

                if (moveright == 0 && moveleft == 0 && moveup == 0 && movedown == 0 && p->speed < 1) {
                        moveup = 1;
                }
                
                if (baseColor != black) {
                if (p->speed < 1) {
                        moveup = 1;
                        moveleft = 0;
                        moveright = 0;
                }
                }

                p->k.keys[KEY_MOVEUP] = moveup;
                p->k.keys[KEY_MOVEDOWN] = movedown;
                p->k.keys[KEY_MOVERIGHT] = moveright;
                p->k.keys[KEY_MOVELEFT] = moveleft;
                
        }

        return ret;
}

/* ------------------------------------------------------ */

int computerIa(struct car_properties *p, struct mapData *mapInfos)
{
	Uint32 color;

	int x, y;
        int r = -1;
	
	if(mapInfos->mpRoad != NULL && p != NULL)

	{
	
		if(mapInfos->mpDecoration != NULL)
		{
			if(SDL_MUSTLOCK(mapInfos->mpDecoration))
				SDL_LockSurface(mapInfos->mpDecoration);
		}
		if(SDL_MUSTLOCK(mapInfos->mpRoad))
			SDL_LockSurface(mapInfos->mpRoad);

		x = (int)roundf(p->car_x);
		y = (int)roundf(p->car_y);


		r = computerView(
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

        return r;
}
