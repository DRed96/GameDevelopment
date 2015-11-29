#ifndef _UI_CLASSES_
#define _UI_CLASSES_
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

enum ui_types
{
	T_label,
	T_image
};

enum img_state
{
	idle_state,
	hover_state,
	click_state
};

class UI_element
{
protected:
	ui_types type;
public:
	//Mouse state methods
	bool isColliding() const;
	bool isClicking() const;
	void dragging();
	//Type Getter
	ui_types getType() const;
	//Render
	virtual void draw(int x, int y, img_state state) const;
public:
	int id;
	SDL_Rect rect;
	
};

struct UI_image : public UI_element
{
	SDL_Texture* idle_image;
	SDL_Texture* hover_image;
	SDL_Texture* clicked_image;
public:
	UI_image();
	//Render
	void draw(int x, int y, img_state state) const;
};


struct UI_label : public UI_image
{
	char* idle_text;
	char* hover_text;
	char* clicked_text;
public:
	UI_label();


};
#endif // !_UI_CLASSES_
