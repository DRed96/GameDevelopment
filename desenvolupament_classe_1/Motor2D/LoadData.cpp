#include "LoadData.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

LoadData::LoadData()
{}

LoadData::~LoadData()
{}

bool LoadData::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

/*
Crear modul i mètode loadfile amb SDL_RWops
Audio i image
*/
SDL_RWops* LoadData::LoadFile_RW(char *filename)
{
	SDL_RWops* toLoad_RW = NULL;

	//Posar les dades del .pak en un SDL_RWops
	toLoad_RW = SDL_RWFromFile(filename, "r");
	if (toLoad_RW==NULL)
	{
		LOG("Failed to create RW from file! ERROR: %s", SDL_GetError());
	}
	
	return toLoad_RW;
}

SDL_Texture* LoadData::LoadImages(char *filename)
{
	SDL_RWops* RW_src = NULL;
	SDL_Texture* ret = NULL;

	RW_src = LoadFile_RW(filename);

	if (RW_src != NULL)
	{
		SDL_Surface* toLoadSurface = IMG_Load_RW(RW_src, 1);
		if (toLoadSurface == NULL)
		{
			LOG("Failed to load IMG from RW! ERROR: %s", IMG_GetError());
		}
		else
		{
			//ret = SDL_CreateTextureFromSurface(App->render->renderer, toLoadSurface);
			//App->tex->textures.add(ret);
			ret = App->tex->LoadSurface(toLoadSurface);
			SDL_FreeSurface(toLoadSurface);
			if (ret == NULL)
			{
				LOG("Failed to load IMG from RW! ERROR: %s", IMG_GetError());
			}
		}
	}
	return ret;
}
Mix_Chunk* LoadData::LoadAudio(char *filename)
{
	Mix_Chunk * ret = NULL;
	SDL_RWops* RW_src = NULL;

	RW_src = LoadFile_RW(filename);

	if (RW_src != NULL)
	{
		ret = Mix_LoadWAV_RW(RW_src, 1);

		if (!ret)
		{
			LOG("Mix_LoadWAV_RW: %s\n", Mix_GetError());
		}
	}
	return ret;
}
