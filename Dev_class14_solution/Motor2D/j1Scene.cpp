#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "Gui.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");
	Gui* window = App->gui->CreateImage({0, 512, 483, 512});
	window->Center();
	window->draggable = true;
	window->interactive = true;
	window->cut_childs = false;

	Gui* input = App->gui->CreateInput({488, 569, 344, 61}, "Your name", 315, {-13,-14});
	input->parent = window;
	input->interactive = true;
	input->Center();
	input->SetLocalPos(input->GetLocalPos().x, 150);
	input->can_focus = true;

	button = App->gui->CreateImage({642, 169, 229, 69});
	button->SetListener(this);
	button->interactive = true;
	button->can_focus = true;
	
	Gui* text = App->gui->CreateLabel("Button");

	text->parent = button;
	button->parent = window;

	text->Center();
	button->Center();

	Gui* title = App->gui->CreateLabel("Window Title");
	title->parent = window;
	title->Center();
	iPoint p = title->GetLocalPos();
	title->SetLocalPos(p.x, 50);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---
	
	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

// Called when UI event is raised
void j1Scene::OnGui(Gui* ui, GuiEvents event)
{

/*Normal Button coords are {0,111,229,69} - hover state
Bright Button coords are {410,169,229,69} - click state
Dark Button coords are {645,165,229,69} - normal state
*/
	if(ui == button)
	{
		switch(event)
		{
			case GuiEvents::mouse_enters:
			case GuiEvents::gain_focus:
			button->SetSection({0,113,229,69});
			break;

			case GuiEvents::mouse_leaves:
			case GuiEvents::lost_focus:
			button->SetSection({642, 169, 229, 69});
			break;

			case GuiEvents::mouse_lclick_down:
			button->SetSection({411, 169, 229, 69});
			break;

			case GuiEvents::mouse_lclick_up :
			button->SetSection({0, 113, 229, 69});
			break;

			case GuiEvents::mouse_rclick_down:
			button->SetSection({411, 169, 229, 69});
			break;

			case GuiEvents::mouse_rclick_up:
			button->SetSection({0, 113, 229, 69});
			break;
		}
	}
}