#include "UI_Classes.h"


//Checks hover
bool UI_element :: isColliding()
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

bool UI_element::isClicking()
{
	return (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN);
}