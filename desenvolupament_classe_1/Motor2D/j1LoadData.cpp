#include "j1LoadData.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
LoadData::LoadData() : toLoad_RW(NULL)
{

}

LoadData::~LoadData()
{
}
/*
Crear modul i mètode loadfile amb SDL_RWops
Audio i image
*/
SDL_RWops* LoadData::LoadFile_RW(char *filename)
{
		//SDL_RWops* toLoad_RW = NULL;

	//Posar les dades del .pak en un SDL_RWops
	toLoad_RW = SDL_RWFromFile(filename, "r");
	if (toLoad_RW==NULL)
	{
		LOG("Failed to create RW from file! ERROR: %s", SDL_GetError());
	}
	
	return toLoad_RW;
}

SDL_Texture* LoadData::LoadTexture(SDL_RWops * src)
{
	SDL_Texture* ret;
	SDL_Surface* loadSurface = IMG_Load_RW(src, 1);
	if (toLoad_RW == NULL)
	{
		LOG("Failed to load IMG from RW! ERROR: %s", IMG_GetError());
	}
	else
	{
		SDL_FreeSurface(loadSurface);
		ret = SDL_CreateTextureFromSurface(App->render->renderer, loadSurface);
	}
	return ret;
}
SDL_AudioSpec* LoadAudio(SDL_RWops * src)
{
	Mix_Chunk * ret = NULL;
	ret = Mix_LoadWAV_RW(src, 1);

	if (!ret) {
		printf("Mix_LoadWAV_RW: %s\n", Mix_GetError());
		// handle error
	}
}
/*

}*/