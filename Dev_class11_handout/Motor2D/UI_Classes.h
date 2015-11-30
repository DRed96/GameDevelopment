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
	virtual void draw( img_state state) const;
public:
	int id;
	//Rects
	SDL_Rect* rect;
};

struct UI_image : public UI_element
{
	SDL_Texture* texture;
	
	//Textures
	SDL_Rect* idle_rect;
	SDL_Rect* hover_rect;
	SDL_Rect* clicked_rect;
	
public:
	UI_image();
	~UI_image();
	//Render
	void draw(img_state state) const;
	//Add Optional Rects
	void addHoverRect(const int x, const int y, const int w, const int h);

	void addClickedRect(const int x, const int y, const int w, const int h);
};


struct UI_label : public UI_element
{
	char* idle_text;
	char* hover_text;
	char* clicked_text;

	SDL_Texture* idle_texture;
	SDL_Texture* hover_texture;
	SDL_Texture* clicked_texture;
public:
	UI_label();
	~UI_label();

};
#endif // !_UI_CLASSES_
