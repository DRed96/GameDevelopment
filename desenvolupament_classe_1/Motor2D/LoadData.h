#ifndef _LOAD_DATA_H_
#define _LOAD_DATA_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL_mixer\include\SDL_mixer.h"
class LoadData: public j1Module
{
public:
	LoadData();
	~LoadData();

	SDL_RWops* LoadFile_RW(char *);
	SDL_Surface* LoadImages(SDL_RWops*);
	Mix_Chunk* LoadAudio(SDL_RWops *);

};



#endif // !_LOAD_DATA_H_
