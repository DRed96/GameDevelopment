#ifndef _UI_CLASSES_
#define _UI_CLASSES_

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
	//Inherited methods
	void draw();
	void update();
	void cleanUp();
};


struct UI_image : public UI_element
{
	SDL_Texture* image;
public:
	//Inherited methods
	void draw();
	void update();
	void cleanUp();

	void handleInput();
};

#endif // !_UI_CLASSES_
