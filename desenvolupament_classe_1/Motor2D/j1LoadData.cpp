#include "j1LoadData.h"
#include "p2Log.h"

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
SDL_RWops* LoadData::LoadFiles/*_RW*/(char *filename)
{
	//Posar les dades del .pak en un SDL_RWops
	toLoad_RW = SDL_RWFromFile(filename, "r");
	if (toLoad_RW==NULL)
	{
		LOG("Failed to create RW from file! ERROR: %s", SDL_GetError());
	}
	else
	{
		IMG_Load_RW(toLoad_RW, 1);
	}
	//Cridad la funció Load de SDL_IMG ?en array¿
}