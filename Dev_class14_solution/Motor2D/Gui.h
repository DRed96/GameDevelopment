#ifndef __GUI_H__
#define __GUI_H__

#include "p2Point.h"

#define CURSOR_WIDTH 2

enum GuiEvents
{
	listening_ends,
	mouse_enters,
	mouse_leaves,
	mouse_lclick_down,
	mouse_lclick_up,
	mouse_rclick_down,
	mouse_rclick_up,
	gain_focus,
	lost_focus,
	input_changed,
	input_submit
};

enum GuiTypes
{
	unknown,
	image,
	label,
	button
};

// ---------------------------------------------------
class Gui
{
public:
	Gui();
	virtual ~Gui()
	{}

	virtual void Draw() const
	{}
	virtual void DebugDraw() const;
	virtual void Update(const Gui* mouse_hover, const Gui* focus)
	{}
	void CheckInput(const Gui* mouse_hover, const Gui* focus);
	void SetLocalPos(int x, int y);
	void Center();
	rectangle GetScreenRect() const;
	rectangle GetLocalRect() const;
	iPoint GetScreenPos() const;
	iPoint GetLocalPos() const;
	void SetListener(j1Module* module);

protected:
	void SetSize(int w, int h);

public:
	bool draggable = false;
	bool interactive = false;
	bool cut_childs = false;
	bool can_focus = false;
	Gui* parent = nullptr;
protected:
	GuiTypes type = GuiTypes::unknown;
	j1Module* listener = nullptr;
	bool have_focus = false;
private:
	bool mouse_inside = false;
	rectangle rect;
};

// ---------------------------------------------------
class GuiImage : public Gui
{
public:
	GuiImage(const SDL_Texture* texture);
	GuiImage(const SDL_Texture* texture, const rectangle& section);
	~GuiImage();

	void SetSection(const rectangle& section);
	void Draw() const;

private:

	rectangle section;
	const SDL_Texture* texture = nullptr;
};

// ---------------------------------------------------
class GuiLabel : public Gui
{
public:
	GuiLabel(const char* text);
	~GuiLabel();

	void SetText(const char* text);
	const SDL_Texture* GetTexture() const;
	void Draw() const;

private:

	SDL_Texture* texture = nullptr;
};

// ---------------------------------------------------
// TODO 1: Create a new InputText UI element with it's own label and image
// and draw it
class GuiInputText : public Gui
{
public:
	GuiInputText(const char* default_text, uint width, const SDL_Texture* texture, const rectangle& section, const iPoint& offset = {0, 0});
	~GuiInputText();

	void Update(const Gui* mouse_hover, const Gui* focus);
	void Draw() const;

private:

	GuiLabel text;
	GuiImage image;
	p2SString input;
	iPoint cursor_coords = {0, 0};
	int last_cursor = 0;
	bool had_focus = false;
};

#endif // __GUI_H__
