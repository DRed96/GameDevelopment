#ifndef _LOAD_DATA_H_
#define _LOAD_DATA_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL_mixer\include\SDL_mixer.h"

#include "j1Physlib.h"
class LoadData: public j1Module
{
public:
	LoadData();
	~LoadData();

	bool Start();
	SDL_RWops* loadFile_PHYSFS(char *filename);
	SDL_RWops* LoadFile_RW(char * filename);
	SDL_Texture* LoadImages(char *filename);
	Mix_Chunk* LoadAudio(char *filename);

};



#endif // !_LOAD_DATA_H_
