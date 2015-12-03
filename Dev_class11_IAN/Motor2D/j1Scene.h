#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class Gui_Image;
class Gui_Label;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Receive GUI_Event
	void ReceiveEvent(Gui_Element* el, Gui_Events ev);

private:
	SDL_Texture* debug_tex;
	Gui_Image* banner;
	Gui_Label* text;
};

#endif // __j1SCENE_H__