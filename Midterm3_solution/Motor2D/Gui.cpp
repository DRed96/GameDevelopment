#include "j1Textures.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Gui.h"
#include "j1App.h"

// class Gui ---------------------------------------------------
Gui::Gui() : rect({0,0,0,0})
{
}

void Gui::SetLocalPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Gui::Center()
{
	int frame_w = (parent) ? parent->GetLocalRect().w : App->render->camera.w;
	int frame_h = (parent) ? parent->GetLocalRect().h : App->render->camera.h;

	SetLocalPos(frame_w / 2 - rect.w / 2, frame_h / 2 - rect.h / 2);
}

void Gui::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}

rectangle Gui::GetLocalRect() const
{
	return rect;
}

rectangle Gui::GetScreenRect() const
{
	if(parent != nullptr)
	{
		iPoint p = GetScreenPos();
		return {p.x, p.y, rect.w, rect.h};
	}
	return rect;
}

iPoint Gui::GetScreenPos() const
{
	if(parent != nullptr)
		return parent->GetScreenPos() + iPoint(rect.x, rect.y);
	else
		return iPoint(rect.x, rect.y);
}

iPoint Gui::GetLocalPos() const
{
	return iPoint(rect.x, rect.y);
}

void Gui::DebugDraw() const
{
	rectangle r = GetScreenRect();
	App->render->DrawQuad({r.x, r.y, r.w, r.h}, 255, (have_focus) ? 255 : 0, 0, 255, false, false);
}

void Gui::SetListener(j1Module* module)
{
	if(listener != nullptr)
		listener->OnGui(this, GuiEvents::listening_ends);

	listener = module;
}

void Gui::CheckInput(const Gui* mouse_hover, const Gui* focus)
{
	bool inside = (mouse_hover == this);

	if(mouse_inside != inside)
	{
		mouse_inside = inside;
		if(listener != nullptr)
			listener->OnGui(this, (inside) ? GuiEvents::mouse_enters : GuiEvents::mouse_leaves);
	}

	if(inside == true)
	{
		if(listener != nullptr)
		{
			if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_DOWN)
				listener->OnGui(this, GuiEvents::mouse_lclick_down);

			if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_UP)
				listener->OnGui(this, GuiEvents::mouse_lclick_up);

			if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == j1KeyState::KEY_DOWN)
				listener->OnGui(this, GuiEvents::mouse_rclick_down);

			if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == j1KeyState::KEY_UP)
				listener->OnGui(this, GuiEvents::mouse_lclick_up);
		}

		if(draggable == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_REPEAT)
		{
			iPoint p = GetLocalPos();
			iPoint motion;
			App->input->GetMouseMotion(motion.x, motion.y);
			SetLocalPos(p.x + motion.x, p.y + motion.y);
		}
	}

	if(have_focus != (focus == this))
	{
		if(listener != nullptr)
		{
			if(focus == this)
				listener->OnGui(this, GuiEvents::gain_focus);
			else
				listener->OnGui(this, GuiEvents::lost_focus);
		}
		have_focus = (focus == this);
	}

	if(have_focus == true && listener != nullptr)
	{
		if(App->input->GetKey(SDL_SCANCODE_RETURN) == j1KeyState::KEY_DOWN)
			listener->OnGui(this, GuiEvents::mouse_lclick_down);

		if(App->input->GetKey(SDL_SCANCODE_RETURN) == j1KeyState::KEY_UP)
			listener->OnGui(this, GuiEvents::mouse_lclick_up);
	}
}

// class GuiImage ---------------------------------------------------
GuiImage::GuiImage(const SDL_Texture* texture) : Gui(), texture(texture)
{
	section.x = section.y = 0;
	App->tex->GetSize(texture, (uint&)section.w, (uint&)section.h);

	SetSize(section.w, section.h);
	type = GuiTypes::image;
}

// --------------------------
GuiImage::GuiImage(const SDL_Texture* texture, const rectangle& section) : Gui(), texture(texture), section(section)
{
	SetSize(section.w, section.h);
}

// --------------------------
GuiImage::~GuiImage()
{}

// --------------------------
void GuiImage::SetSection(const rectangle& section)
{
	this->section = section;
}

// --------------------------
void GuiImage::Draw() const
{
	iPoint p = GetScreenPos();
	if(parent && parent->cut_childs)
	{
		rectangle r = parent->GetScreenRect();
		App->render->SetViewPort({r.x, r.y, r.w, r.h});
		p = GetLocalPos();
	}
	App->render->Blit(texture, p.x, p.y, (SDL_Rect*) &section, 0.0f);

	if(parent && parent->cut_childs)
		App->render->ResetViewPort();
}

// class GuiLabel ---------------------------------------------------
GuiLabel::GuiLabel(const char* text) : Gui()
{
	SetText(text);
	type = GuiTypes::label;
}

// --------------------------
GuiLabel::~GuiLabel()
{
	if(texture != nullptr && texture != App->gui->GetAtlas())
		App->tex->UnLoad(texture);
}

// --------------------------
void GuiLabel::SetText(const char* text)
{
	if(texture != nullptr)
		SDL_DestroyTexture(texture);

	texture = App->font->Print(text);
	int w, h;
	App->tex->GetSize(texture, (uint&)w, (uint&)h);
	SetSize(w, h);
}

// --------------------------
const SDL_Texture* GuiLabel::GetTexture() const
{
	return texture;
}

// --------------------------
void GuiLabel::Draw() const
{
	iPoint p = GetScreenPos();
	App->render->Blit(texture, p.x, p.y, NULL, 0.0f);
}

// class GuiInputText ---------------------------------------------------
GuiInputText::GuiInputText(const char* default_text, uint width, const SDL_Texture* texture, const rectangle& section, const iPoint& offset) 
	: Gui(), text(default_text), input(default_text), image(texture, section)
{
	type = GuiTypes::label;
	SetSize(width, text.GetScreenRect().h);
	text.parent = this;
	image.parent = this;
	image.SetLocalPos(offset.x, offset.y);

	// Calculate the Y so we have it ready
	App->font->CalcSize("A", cursor_coords.x, cursor_coords.y);
	cursor_coords.x = 0;
}

// --------------------------
GuiInputText::~GuiInputText()
{
}

// --------------------------
void GuiInputText::Update(const Gui* mouse_hover, const Gui* focus)
{
	if(interactive == false)
		return;

	bool inside = (mouse_hover == this);
	bool have_focus = (focus == this);

	if(had_focus != have_focus)
	{
		if(have_focus == true)
			App->input->StartTextInput(nullptr);
		else
			App->input->EndTextInput();

		had_focus = have_focus;
	}

	static p2SString selected(100);

	if(have_focus == true)
	{
		int cursor, selection;
		const char* user_input = App->input->GetTextInput(cursor, selection);
		if(input != user_input || cursor != last_cursor)
		{
			if(input != user_input)
			{
				input = user_input;
				text.SetText(user_input);
				if(listener != nullptr)
					listener->OnGui(this, GuiEvents::input_changed);
			}

			last_cursor = cursor;
			if(cursor > 0)
			{
				if(input.Length() >= selected.GetCapacity())
					selected.Reserve(input.Length() * 2);
				input.SubString(0, cursor, selected);
				App->font->CalcSize(selected.GetString(), cursor_coords.x, cursor_coords.y);
			}
			else
			{
				cursor_coords.x = 0;
			}
		}

		if(selection != 0 && listener != nullptr)
		{
			listener->OnGui(this, GuiEvents::input_submit);
		}
	}
}

// --------------------------
void GuiInputText::Draw() const
{
	// render image
	image.Draw();

	// render text
	if(input.Length() > 0)
		text.Draw();

	// render cursor
	if(have_focus == true)
	{
		iPoint pos = GetScreenPos();
		App->render->DrawQuad({pos.x + (cursor_coords.x - (CURSOR_WIDTH / 2)), pos.y, CURSOR_WIDTH, cursor_coords.y}, 255, 255, 255, 255, true, false);
	}
}

// class GuiHScrollBar ---------------------------------------------------
// EXERCISE 1
GuiHScrollBar::GuiHScrollBar(const rectangle& bar, const rectangle& offset, const rectangle& thumb, iPoint margins) :
bar(App->gui->GetAtlas(), bar), thumb(App->gui->GetAtlas(), thumb), margins(margins)
{
	type = GuiTypes::hscrollbar;
	SetSize(bar.w + offset.w, bar.h + offset.h);
	this->bar.parent = this;
	this->thumb.parent = this;
	this->bar.SetLocalPos(offset.x, offset.y);
	this->thumb.SetLocalPos(margins.x, margins.y);

	// EXERCISE 6
	min_x = margins.x;
	max_x = bar.w + offset.w - margins.x - thumb.w;
}

GuiHScrollBar::~GuiHScrollBar()
{}

// EXERCISE 2: The thumb should be draggable with the mouse.
void  GuiHScrollBar::Update(const Gui* mouse_hover, const Gui* focus)
{
	int requested_change = 0;

	if(focus == this)
	{
		// EXERCISE 3
		if(App->input->GetKey(SDL_SCANCODE_LEFT) == j1KeyState::KEY_REPEAT)
		{
			requested_change = -1;
		}

		if(App->input->GetKey(SDL_SCANCODE_RIGHT) == j1KeyState::KEY_REPEAT)
		{
			requested_change = 1;
		}
	}

	if(mouse_hover == this)
	{
		if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_REPEAT)
		{
			iPoint mouse;
			App->input->GetMousePosition(mouse.x, mouse.y);
			if(thumb.GetScreenRect().Contains(mouse.x, mouse.y))
			{
				// EXERCISE 2
				iPoint motion;
				App->input->GetMouseMotion(motion.x, motion.y);
				requested_change = motion.x;
			}
			else
			{
				// EXERCISE 4
				iPoint pos = thumb.GetScreenPos();
				if(mouse.x < pos.x)
					requested_change = -1;
				else
					requested_change = 1;
			}
		}
	}

	if(requested_change != 0)
	{
		iPoint p = thumb.GetLocalPos();
		int x = MIN(max_x, p.x + requested_change);
		if(x < min_x) x = min_x;

		if(x != p.x)
		{
			// EXERCISE 5
			if(listener != NULL)
				listener->OnGui(this, GuiEvents::value_changed);
			thumb.SetLocalPos(x, p.y);
		}
	}
}

// EXERCISE 1
void  GuiHScrollBar::Draw() const
{
	bar.Draw();
	thumb.Draw();
}

// EXERCISE 5
float GuiHScrollBar::GetValue() const
{
	iPoint p = thumb.GetLocalPos();
	return float(p.x - min_x) / float(max_x - min_x);
}

