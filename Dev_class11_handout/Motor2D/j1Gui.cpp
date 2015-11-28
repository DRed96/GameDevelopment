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
	current = mouse_idle;
	collided = NULL;
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
	guis.clear();
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
	
	guis.add(ret);
	return ret;
}
/*UI_image* createImage(SDL_Texture* image, SDL_Rect rect)
{
	UI_image* ret;
	ret = new UI_image();

	ret->image = image;
	ret->rect = rect;
	
	//labels.Add(ret);
	return ret;
}*/

UI_label* j1Gui::createLabel(char* text, int x, int y, int w, int h)
{
	UI_label* ret;
	ret = new UI_label();

	ret->text = text;
	ret->image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
	
	//Fill rect
	ret->rect.x = x;
	ret->rect.y = y;
	ret->rect.w = strlen(text) + 1;
	ret->rect.h = h;

	guis.add(ret);
	return ret;
}
/*
UI_label* createLabel(char* text, SDL_Rect rect)
{
	UI_label* ret;
	ret = new UI_label();

	ret->text = text;
	ret->rect = rect;

	//images.Add(ret);
	return ret;
}
*/

void j1Gui::mouseState()
{
	p2List_item <UI_element*>* tmp = guis.start;

	bool isInside = false;

	while (tmp && collided == NULL)
	{
		isInside = tmp->data->isColliding();
		if (isInside)
			collided = tmp,
		tmp = tmp->next;
	}
	if (isInside)
	{
		guiReviever(mouse_click, collided->data);
		collided->data->isColliding();
		LOG("MOUSE IS INSIDE!");
	}
	
}
/*while (tmp)
	{
		switch (current)
		{
		case mouse_enter:
			if (x < tmp->data->rect.x || x > tmp->data->rect.x + tmp->data->rect.w
				|| y < tmp->data->rect.y || y > tmp->data->rect.y + tmp->data->rect.h)
			{
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
					{
						guiReviever(mouse_click, tmp->data);
						LOG("CLICKED!");
					}
					current = mouse_leave;
					guiReviever(current, tmp->data);
					LOG("MOUSE IS GONE!");
			}
			break;
		
		default:
			if (x > tmp->data->rect.x && x < tmp->data->rect.x + tmp->data->rect.w 
				&& y > tmp->data->rect.y && y < tmp->data->rect.y + tmp->data->rect.h)
			{
				current = mouse_enter;
				guiReviever(current, tmp->data);
				LOG("MOUSE IS INSIDE!");
			}
			break;
		}
		tmp = tmp->next;
		}*/