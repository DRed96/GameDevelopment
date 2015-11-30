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

	//Render
	void render() const;

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	//They create a basic UI_image with a single texture
	UI_image* createImage(SDL_Texture* image, int x, int y, int w, int h);
	UI_image* createImage(SDL_Texture* image, SDL_Rect* rect, SDL_Rect* texture_rect,
						SDL_Rect* _hover_rect = NULL, SDL_Rect* _click_rect = NULL);

	UI_label* createLabel(char* text, int x, int y, int w = 12, int h = 12, img_state type = idle_state);//12 is the standard typo size
	//Unrecomended method vvvvvvv
	UI_label* createLabel(char* text, SDL_Rect* rect, img_state type = idle_state);
		
	//Checks mouse position respect the UI elements
	void mouseState();
	//Does nothing
	void guiReviever(gui_events g_event, UI_element * element);

	const SDL_Texture* GetAtlas() const;



private:
	//All the list of UI elements
	p2List<UI_element*> guis;
	//Saves the current event
	gui_events current;
	//Saves the UI element in witch the button is
	p2List_item <UI_element*>* collided;

	//j1KeyState mouse_state;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__