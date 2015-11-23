#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
UI_image* j1Gui::createImage(SDL_Texture* image, int x, int y, int w, int h)
{
	UI_image* ret;
	ret = new UI_image();

	ret->image = image;
	ret->rect.x = x;
	ret->rect.y = y;
	ret->rect.w = w;
	ret->rect.h = h;

	return ret;
}
UI_image* createImage(SDL_Texture* image, SDL_Rect rect)
{
	UI_image* ret;
	ret = new UI_image();

	ret->image = image;
	ret->rect = rect;

	return ret;
}

UI_label* createLabel(char* text, int x, int y, int w, int h)
{
	UI_label* ret;
	ret = new UI_label();

	ret->text = text;
	//Fill rect
	ret->rect.x = x;
	ret->rect.y = y;
	ret->rect.w = w;
	ret->rect.h = h;

	return ret;
}
UI_label* createLabel(char* text, SDL_Rect rect)
{
	UI_label* ret;
	ret = new UI_label();

	ret->text = text;
	ret->rect = rect;

	return ret;
}
