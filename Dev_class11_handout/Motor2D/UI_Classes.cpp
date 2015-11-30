#include "UI_Classes.h"

//General methods-------------
void UI_element::draw(img_state state) const
{
	return;
}
//Checks hover
bool UI_element :: isColliding() const
{
	bool ret = false;
	int m_x, m_y;
	App->input->GetMousePosition(m_x,m_y);

	//Check if it is inside the rect
	if (m_x > rect->x && m_x < (rect->x + rect->w) && m_y > rect->y && m_y < (rect->y + rect->h))
	{
		ret = true;
	}

	return ret;
}

bool UI_element::isClicking() const
{
	bool ret = false;
	j1KeyState mouse_state = App->input->GetMouseButtonDown(SDL_BUTTON_LEFT);
		if (mouse_state == KEY_DOWN || mouse_state == KEY_REPEAT)
			ret = true;
	return ret;
}

void UI_element::dragging()
{
	/*
	*The function should create a faded image of the button that you could move
	*/
	return;
}

ui_types UI_element::getType() const
{
	return type;
}

//Image Methods----------------

UI_image::UI_image() : texture(NULL), idle_rect(NULL), hover_rect(NULL), clicked_rect(NULL)
{
	type = T_image;
}

UI_image::~UI_image()
{
	//Release all rects
	RELEASE(idle_rect);
	RELEASE(hover_rect);
	RELEASE(clicked_rect);
	//Release Texture
	RELEASE(texture);
}
//Render
void UI_image::draw( img_state state) const
{
	SDL_Rect* texture_rect = idle_rect;
	switch (state)
	{
	case hover_state:
		texture_rect = hover_rect;
		break;
	case click_state:
		texture_rect = clicked_rect;
		break;
	}
	App->render->Blit(texture, rect->x, rect->y, texture_rect);
}

void UI_image::addHoverRect(const int x, const int y, const int w, const int h)
{
	if (hover_rect == NULL)
	{
		hover_rect = new SDL_Rect;
	}
	hover_rect->x = x;
	hover_rect->y = y;
	hover_rect->w = w;
	hover_rect->h = h;
}

void UI_image::addClickedRect(const int x, const int y, const int w, const int h)
{
	if (clicked_rect == NULL)
	{
		clicked_rect = new SDL_Rect;
	}
	clicked_rect->x = x;
	clicked_rect->y = y;
	clicked_rect->w = w;
	clicked_rect->h = h;
}
//Label Methods----------------

UI_label::UI_label() : idle_text(NULL), hover_text(NULL), clicked_text(NULL)
{
	/*idle_image = NULL;
	hover_image = NULL; 
	clicked_image = NULL;*/
	type = T_label;
}

//Render
void UI_label::draw(img_state state) const
{
	SDL_Texture* current_t = idle_texture;
	switch (state)
	{
	case hover_state:
		if (hover_texture)
			current_t = hover_texture;
		break;
	case click_state:
		if (clicked_texture)
			current_t = clicked_texture;
		break;
	}
	App->render->Blit(current_t, rect->x, rect->y);
}


//Add Optional Text Textures
void UI_label::addHoverText(char* _text)
{
	hover_text = _text;
	hover_texture =  App->font->Print(_text, { 255, 255, 255, 255 }, App->font->default);
}

void UI_label::addClickedText(char* _text)
{
	clicked_text = _text;
	clicked_texture = App->font->Print(_text, { 255, 255, 255, 255 }, App->font->default);
}