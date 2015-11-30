#ifndef _UI_CLASSES_
#define _UI_CLASSES_
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Fonts.h"
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
	int id;
	//Rects
	SDL_Rect* rect;
public:
	//Mouse state methods
	bool isColliding() const;
	bool isClicking() const;
	void dragging();
	//Type Getter
	ui_types getType() const;
	//Render
	virtual void draw(SDL_Texture*, img_state state) const;

};

struct UI_image : public UI_element
{	
	//Textures
	SDL_Rect* idle_rect;
	SDL_Rect* hover_rect;
	SDL_Rect* clicked_rect;
	
public:
	UI_image();
	~UI_image();
	//Render
	void draw( SDL_Texture*, img_state state) const;
	//Add Optional Rects
	void addHoverRect(const int x, const int y, const int w, const int h);

	void addClickedRect(const int x, const int y, const int w, const int h);
};


struct UI_label : public UI_element
{
	//Text strings
	char* idle_text;
	char* hover_text;
	char* clicked_text;

	//A texture for every text
	SDL_Texture* idle_texture;
	SDL_Texture* hover_texture;
	SDL_Texture* clicked_texture;

	//Render
	void draw(SDL_Texture*, img_state state) const;
public:
	UI_label();
	~UI_label();

	//Add Optional Text Textures
	void addHoverText(char* _text);

	void addClickedText(char* _text);

};
#endif // !_UI_CLASSES_
