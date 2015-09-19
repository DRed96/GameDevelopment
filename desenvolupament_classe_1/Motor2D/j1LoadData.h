#ifndef _LOAD_DATA_H_
#define _LOAD_DATA_H_

#include "j1Module.h"
#include "SDL/include/SDL.h"
#include "SDL_image\include\SDL_image.h"
class LoadData
{
public:
	LoadData();
	~LoadData();

	SDL_RWops* LoadFiles(char *);
private:
	SDL_RWops* toLoad_RW = NULL;
};



#endif // !_LOAD_DATA_H_
