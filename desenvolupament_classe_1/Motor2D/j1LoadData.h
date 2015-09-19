#ifndef _LOAD_DATA_H_
#define _LOAD_DATA_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL_mixer\include\SDL_mixer.h"
class LoadData
{
public:
	LoadData();
	~LoadData();

	SDL_RWops* LoadFile_RW(char *);
	SDL_Texture* LoadTexture(SDL_RWops*);
	SDL_AudioSpec* LoadAudio(SDL_RWops *);
private:
	SDL_RWops* toLoad_RW = NULL;
};



#endif // !_LOAD_DATA_H_
