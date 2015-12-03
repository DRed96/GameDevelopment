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
	//Deletes
	//Gui_Image
	p2List_item<Gui_Image*>* tmp_i = image_list.start;
	while (tmp_i != NULL)
	{
		delete tmp_i->data;
		tmp_i = tmp_i->next;
	}

	//Gui_Label
	p2List_item<Gui_Label*>* tmp_l = label_list.start;
	while (tmp_l != NULL)
	{
		delete tmp_l->data;
		tmp_l = tmp_l->next;
	}

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

//Factory functions
//Gui_Image
Gui_Image* j1Gui::CreateGuiImage(SDL_Rect rect, p2Point<int> position)
{
	Gui_Image* ret;

	ret = new Gui_Image(position, rect);

	image_list.add(ret);

	return ret;
}

//Gui_Label
Gui_Label* j1Gui::CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position)
{
	Gui_Label* ret;

	ret = new Gui_Label(position, text, font);

	label_list.add(ret);

	return ret;
}

Gui_Label* j1Gui::CreateGuiLabel(p2SString text, _TTF_Font* font, p2Point<int> position, SDL_Color color)
{
	Gui_Label* ret;

	ret = new Gui_Label(position, text, font, color);

	label_list.add(ret);

	return ret;
}

// class Gui ---------------------------------------------------

//Gui_Element
Gui_Element::Gui_Element(p2Point<int> pos, Gui_Type t)
{
	position = pos;
	type = t;
	mouseInside = false;
	mouseInsidePrev = false;
	module_list.add((j1Module*)App->scene);
}

Gui_Element::Gui_Element(p2Point<int> pos, SDL_Rect r, Gui_Type t)
{
	position = pos;
	rect = r;
	type = t;
	mouseInside = false;
	mouseInsidePrev = false;
	module_list.add((j1Module*)App->scene);
}

Gui_Element::~Gui_Element()
{

}

bool Gui_Element::MouseCollision(p2Point<int> mouse_pos)
{
	bool ret = false;

	if (mouse_pos.x > position.x &&
		mouse_pos.x < rect.w + position.x &&
		mouse_pos.y > position.y &&
		mouse_pos.y < rect.h + position.y)
	{
		ret = true;
	}

	return ret;
}

//Gui_Image
Gui_Image::Gui_Image(p2Point<int> pos, SDL_Rect r) : Gui_Element(pos, r, GUI_IMAGE)
{
	texture = App->gui->GetAtlas();
}

Gui_Image::~Gui_Image()
{

}

void Gui_Image::Update()
{
	//Mouse Position
	int mouse_x = 0;
	int mouse_y = 0;
	App->input->GetMousePosition(mouse_x, mouse_y);

	p2List_item<j1Module*>* tmp = module_list.start;
	while (tmp != NULL)
	{
		//Check if, when outside, mouse enters
		if (MouseCollision({ mouse_x, mouse_y }) &&
			mouseInside == false)
		{
			tmp->data->ReceiveEvent(this, MOUSE_ENTER);
		}
		//Check if, when inside, mouse leaves
		else if (MouseCollision({ mouse_x, mouse_y }) == false)
		{
			tmp->data->ReceiveEvent(this, MOUSE_LEAVE);
		}
		tmp = tmp->next;
	}

	Draw();
}

void Gui_Image::Draw()
{
	App->render->Blit(texture, position.x, position.y, &rect, 0.0f);
}

//Gui_Label
Gui_Label::Gui_Label(p2Point<int> pos, p2SString t, _TTF_Font* f, SDL_Color c) : Gui_Element(pos, GUI_LABEL)
{
	text = t;
	font = f;
	color = c;
	texture = App->font->Print(text.GetString(), color, font);
	
	rect.x = 0;
	rect.y = 0;
	App->font->CalcSize(text.GetString(), rect.w, rect.h, font);
}

Gui_Label::~Gui_Label()
{

}

void Gui_Label::Update()
{
	int mouse_x = 0;
	int mouse_y = 0;
	App->input->GetMousePosition(mouse_x, mouse_y);

	p2List_item<j1Module*>* tmp = module_list.start;
	while (tmp != NULL)
	{
		//Check if, when outside, mouse enters
		if (MouseCollision({ mouse_x, mouse_y }) &&
			mouseInside == false)
		{
			tmp->data->ReceiveEvent(this, MOUSE_ENTER);
		}
		//Check if, when inside, mouse leaves
		else if (MouseCollision({ mouse_x, mouse_y }) == false)
		{
			tmp->data->ReceiveEvent(this, MOUSE_LEAVE);
		}
		tmp = tmp->next;
	}

	Draw();
}

void Gui_Label::Draw()
{
	App->render->Blit(texture, position.x, position.y, NULL, 0.0f);
}

void Gui_Label::ChangeText(p2SString new_text)
{
	text = new_text;
	texture = App->font->Print(text.GetString(), color, font);
	App->font->CalcSize(text.GetString(), rect.w, rect.h, font);
}