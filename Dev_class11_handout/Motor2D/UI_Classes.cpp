#include "UI_Classes.h"

//General methods-------------
void UI_element::draw(int x, int y, img_state state) const
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
	if (m_x > rect.x && m_x < (rect.x + rect.w) && m_y > rect.y && m_y < (rect.y + rect.h))
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

UI_image::UI_image() : idle_image(NULL), hover_image(NULL), clicked_image(NULL)
{
	type = T_image;
}

//Render
void UI_image::draw(int x, int y, img_state state) const
{
	SDL_Texture*  currentTex = idle_image;
	switch (state)
	{
	case hover_state:
		currentTex = hover_image;
		break;
	case click_state:
		currentTex = clicked_image;
		break;
	}
	App->render->Blit(currentTex, x, y, &rect);
}

//Label Methods----------------

UI_label::UI_label() : idle_text(NULL), hover_text(NULL), clicked_text(NULL)
{
	idle_image = NULL;
	hover_image = NULL; 
	clicked_image = NULL;
	type = T_label;
}