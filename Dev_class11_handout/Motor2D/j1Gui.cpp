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
	mouseState();
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

//Render
void j1Gui::render() const
{
	p2List_item <UI_element*>* tmp = guis.start;
	img_state state = idle_state;
	while (tmp != NULL)
	{
		//if (tmp == collided)
		
		switch (current)
		{
		case mouse_enter:
			state = hover_state;
				break;
		case mouse_click:
			state = click_state;
				break;
		}
		tmp->data->draw(state);
		tmp = tmp->next;
	}
	
}

// class Gui ---------------------------------------------------

//Image factory methods
UI_image* j1Gui::createImage(SDL_Texture* image, int x, int y, int w, int h)
{
	UI_image* ret;
	ret = new UI_image();

	
	ret->idle_image = image;
	
	ret->rect->x = x;
	ret->rect->y = y;
	ret->rect->w = w;
	ret->rect->h = h;
	
	guis.add(ret);
	return ret;
}
UI_image* j1Gui::createImage(SDL_Texture* image, SDL_Rect* _rect, SDL_Rect* _texture_rect)
{
	UI_image* ret;
	ret = new UI_image();

	ret->idle_image = image;	
	ret->rect = new SDL_Rect(*_rect);
	ret->idle_rect = new SDL_Rect(*_texture_rect);
	/*ret->hover_rect = new SDL_Rect(*_hover_rect);
	ret->clicked_rect = new SDL_Rect(*_click_rect);*/
	//labels.Add(ret);
	guis.add(ret);
	return ret;
}

//Label factory methods
UI_label* j1Gui::createLabel(char* text, int x, int y, int w, int h, img_state type)
{
	UI_label* ret;
	ret = new UI_label();

	/*switch (type)
	{
	case idle_state:
		ret->idle_text = text;
		ret->idle_image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
		break;
	case hover_state:
		ret->hover_text = text;
		ret->hover_image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
		break;
	case click_state:
		ret->clicked_text = text;
		ret->clicked_image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
		break;
	}*/
	//Fill rect
	ret->rect->x = x;
	ret->rect->y = y;
	ret->rect->w = strlen(text) * w;
	ret->rect->h = h;

	guis.add(ret);
	return ret;
}
//Unrecomended method vvvvvvv
UI_label* j1Gui::createLabel(char* text, SDL_Rect* rect, img_state type)
{
	UI_label* ret;
	ret = new UI_label();
	/*
	switch (type)
	{
	case idle_state:
		ret->idle_text = text;
		ret->idle_image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
		break;
	case hover_state:
		ret->hover_text = text;
		ret->hover_image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
		break;
	case click_state:
		ret->clicked_text = text;
		ret->clicked_image = App->font->Print(text, { 255, 255, 255, 255 }, App->font->default);
		break;
	}*/

	ret->rect = rect;
	guis.add(ret);
	return ret;
}



void j1Gui::mouseState()
{
	p2List_item <UI_element*>* tmp = guis.start;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		char debug = '\0';
	}
	switch (current)
	{
	default:
		while (tmp != NULL && collided == NULL)
		{
			//What will happen if the cursor is inside a UI element
			if (tmp->data->isColliding())
			{
				collided = tmp;
				current = mouse_enter;
				guiReviever(current, collided->data);
				LOG("MOUSE IS INSIDE!");
			}
			tmp = tmp->next;
		}
			
		break;
	case mouse_drag:
		if (collided->data->isClicking() == true)
		{
			collided->data->dragging();
		}
		else
		{
			current = mouse_idle;
			collided = NULL;
			LOG("MOUSE HAS STOPPED DRAGGING");
		}
		break;
	case mouse_click:
		if (collided->data->isClicking() == false)
		{
			current = mouse_enter;
			guiReviever(current, collided->data);
			LOG("MOUSE IS UNCLICKED!");
		}
		else
		{
			if (collided->data->isColliding() == false)
			{
				current = mouse_drag;
				guiReviever(current, collided->data);
				LOG("MOUSE IS DRAGGING!");
			}
		}
		break;
	case mouse_enter:
	
		if (collided->data->isColliding() == false)
		{
			current = mouse_leave;
			guiReviever(current, collided->data);
			collided = NULL;
			LOG("MOUSE IS OUTSIDE!");
		}
		else
		{
			if (collided->data->isClicking() == true)
			{
				current = mouse_click;
				guiReviever(current, collided->data);
				LOG("MOUSE IS CLICKED!");
			}
		}
		break;

	}
}

//We use various switches in case the function depends on the type of the UI element
void  j1Gui::guiReviever(gui_events g_event, UI_element * element)
{
	ui_types type = element->getType();
	//Is this correct?
	switch (type)
	{
		case T_image:
			UI_image* ptr = (UI_image*)element;
			switch (g_event)
			{
			case mouse_click:
			//	ptr->image =
				break;
			}
		break;
	}
}