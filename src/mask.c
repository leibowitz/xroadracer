#include <SDL/SDL.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 	rmask = 0xff000000,
	gmask = 0x00ff0000,
	bmask = 0x0000ff00,
	amask = 0x000000ff;
#else
Uint32 	rmask = 0x000000ff,
	gmask = 0x0000ff00,
	bmask = 0x00ff0000,
	amask = 0xff000000;
#endif
