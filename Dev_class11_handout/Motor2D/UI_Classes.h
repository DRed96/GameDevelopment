#ifndef _UI_CLASSES_
#define _UI_CLASSES_
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
class UI_element
{
public:
	bool isColliding();
public:
	int id;
	int listener;
	SDL_Rect rect;
	bool inside;
	bool focus;
};


struct UI_label : public UI_element
{
	char* text;
	SDL_Texture* image;
public:

};


struct UI_image : public UI_element
{
	SDL_Texture* image;
public:


	void handleInput();
};

#endif // !_UI_CLASSES_
