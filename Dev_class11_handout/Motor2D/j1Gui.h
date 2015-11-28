#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UI_Classes.h"

#define CURSOR_WIDTH 2

#include "p2DynArray.h"
// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	UI_label* createLabel(char* text, int x, int y, int w = 0, int h = 1);
//	UI_label* createLabel(char* text, SDL_Rect rect);

	UI_image* createImage(SDL_Texture* image, int x, int y,int w, int h);
//	UI_image* createImage(SDL_Texture* image, SDL_Rect rect);

	const SDL_Texture* GetAtlas() const;

	void mouseState();

private:
//Not quite a good solution
	p2List<UI_element*> guis;
	gui_events current;

	p2List_item <UI_element*>* collided;

	j1KeyState mouse_state;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__