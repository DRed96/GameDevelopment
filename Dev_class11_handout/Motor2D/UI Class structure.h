class UI_element
{
	int id;
};

struct UI_label : public UI_element
{
	SDL_Rect rect;
	char* text;

public:
	//Inherited methods
	void draw();
	void update();
	void cleanUp();

	// Destructor
	~UI_label();
};

UI_label::~UI_label()
{
	if (text != NULL)
		delete text;
}
struct UI_image : public UI_element
{
	SDL_Rect rect;
	SDL_Texture* image;
public:
	//Inherited methods
	void draw();
	void update();
	void cleanUp();
	//
	void handleInput();
	// Destructor
	~UI_image();
};

UI_image::~UI_image()
{
	if (image != NULL)
		delete image;
}