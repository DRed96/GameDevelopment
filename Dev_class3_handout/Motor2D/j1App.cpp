#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1FileSystem.h"
#include "j1App.h"

#include "PhysFS/include/physfs.h"
// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	fs = new j1FileSystem();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	//AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();

	config_file.reset();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	bool ret = LoadConfig();
	// self-config
	title.create(app_config.child("title").child_value());
	organization.create(app_config.child("organization").child_value());

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			const char* debug = item->data->name.GetString();
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret == true;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
bool j1App::LoadConfig()
{
	bool ret = true;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = config_file.child("config");
		app_config = config.child("app");
	}

	return ret;
}

bool j1App::loadData()
{
	bool ret = true;

	char* buf;
	int size = App->fs->Load("data_files.xml", &buf);
	pugi::xml_parse_result result = load_file.load_buffer(buf, size);
	RELEASE(buf);
	if (result == NULL)
	{
		LOG("Could not load map xml file data_files.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		load = load_file.child("saved_data");
		//app_save =save.child("app");
	}

	return ret;
}
// ---------------------------------------------
void j1App::PrepareUpdate()
{
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	// TODO 1: This is a good place to call load / Save functions
	if (want_to_save)
	{
		SaveGameNow();
		want_to_save = false;
		
	}

	if (want_to_load)
	{
		LoadGameNow();
		want_to_load = false;
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

void j1App::doSave(const char* filename)
{
	want_to_save = true;
	save_game.create(filename);
}

void j1App::doLoad(const char* filename)
{
	want_to_load = true;
	load_game.create(filename);
}

// TODO 3: Create a simulation of the xml file to read 

// TODO 4: Create a method to actually load an xml file
// then call all the modules to load themselves
bool j1App::LoadGameNow()
{
	bool ret = loadData();

	p2List_item<j1Module*>* item;
	item = modules.start;

	const char* debug = NULL;
//	item->data->loadNow(save);
		
	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		debug = item->data->name.GetString();
	
		ret = item->data->loadNow(load);
	}

	if (!ret)
		LOG("Error at function loadNow File %s", debug);

	return ret;
}


// TODO 7: Create a method to save the current state
// First fill a pugui::xml_document
// Then put it all in memory with
// std::stringstream stream;
// my_xml_document.save(stream);
// then access it via stream.str().c_str()
bool j1App::SaveGameNow()
{

	bool ret = true;
	std::stringstream data_stream;
	char * buff = NULL;
	
	const char* debug = NULL;
	save = save_file.append_child("game_state");
	p2List_item<j1Module*>* item;
	item = modules.start;
	while (item != NULL && ret == true)
	{
		debug = item->data->name.GetString();
		ret = item->data->saveNow(save.append_child(item->data->name.GetString()));
		item = item->next;
	}
	

	unsigned int size = sizeof(save_file);
	//When save_file is full of data, we pass it to a stringstream
	save_file.save(data_stream);
	App->fs->Save("data_files1.xml", data_stream.str().c_str(), size);

	PHYSFS_file* debug2 = PHYSFS_openWrite("data_files1.xml");
	if (!ret)
		LOG("Error at function saveNow. File %s", debug);
	return ret;
}