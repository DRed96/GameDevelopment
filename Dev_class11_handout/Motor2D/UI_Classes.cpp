#include "UI_Classes.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

//Checks hover
bool UI_element :: isColliding()
{
	bool ret = false;
	int m_x, m_y;
	App->input->GetMousePosition(m_x,m_y);
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		char debug = '\0';
	}

	//Check if it is inside the rect
	if (m_x > rect.x && m_x < (rect.x + rect.w) && m_y > rect.y && m_y > (rect.y + rect.h))
	{
		ret = true;
	}

	return ret;
}
