#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

struct _TTF_Font;

// TODO 1: Create your structure of classes
enum Gui_Events
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	UNKNOWN_EVENT
};

enum Gui_Type
{
	GUI_IMAGE,
	GUI_LABEL
};

class Gui_Element
{
public:
	Gui_Element(p2Point<int> pos, Gui_Type type);
	Gui_Element(p2Point<int> pos, SDL_Rect rect, Gui_Type type);
	~Gui_Element();
	virtual void Update(){};
	virtual void Draw(){};

	bool MouseCollision(p2Point<int> mouse_pos);
public:
	p2Point<int>		position;
	SDL_Texture*		texture;
	SDL_Rect			rect;
	Gui_Type			type;
	bool				active;
	bool				mouseInside;
	bool				mouseInsidePrev;
	p2List<j1Module*>	module_list;
};

class Gui_Image : public Gui_Element
{
public:
	Gui_Image(p2Point<int> pos, SDL_Rect r);
	~Gui_Image();
	void Update();
	void Draw();
public:

};

class Gui_Label : public Gui_Element
{
public:
	Gui_Label(p2Point<int> pos, p2SString t, _TTF_Font* f, SDL_Color c = {255, 255, 255, 255});
	~Gui_Label();
	void Update();
	void Draw();
	void ChangeText(p2SString new_text);
public:
	p2SString text;
	_TTF_Font* font;
	SDL_Color color;
};

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
	//Image
	Gui_Image* CreateGuiImage(SDL_Rect rect, p2Point<int> position);
	//Label (default color, white)
	Gui_Label* CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position);
	//Label (with color)
	Gui_Label* CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position, SDL_Color color);

	SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<Gui_Image*> image_list;
	p2List<Gui_Label*> label_list;
};

#endif // __j1GUI_H__