#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser\n");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map\n");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map

	mapVars.tileVars.~p2List();

	//Free the map_vars memory

	//delete(&mapVars);

	// Remove all tilesets


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char* buf;
	int size = App->fs->Load(tmp.GetString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		//Fill the map struct with data(without filling the tileset)
		loadMapHeader();
		loadTilesetHeader();
		
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	
	// TODO 5: LOG all the data loaded
	// iterate all tilesets and LOG everything
	
	
		char* orient = NULL;
		char * renderO = NULL;
		if (mapVars.orientation == 0)
			orient = "orthogonal";
		else if (mapVars.orientation == 1)
			orient = "isometric";
		else if (mapVars.orientation == 2)
			orient = "straggered";
		
		
		if (mapVars.render_order == 0)
			renderO = "right-down";
		else if (mapVars.render_order == 1)
			renderO = "right-up";
		else if (mapVars.render_order == 2)
			renderO = "left-down";
		else if (mapVars.render_order == 3)
			renderO = "left-up";

		LOG("Map Data-----\n Width: %i, Height: %i, Tile width: %i, Tile Height: %i \n, Orientation: %s, Render Order:  %s,, nextObjectId: \n", mapVars.width, mapVars.height, mapVars.tileWidth, mapVars.tileHeight, orient, renderO, mapVars.nextObjectId);
		LOG("Background Color: %f\n", mapVars.backgroundColor);
		const unsigned int limit = mapVars.tileVars.count() - 1;
		for (int i = 0; i <= limit; i++)
		{
			LOG("Tile Data-----\n FirstGid: %i, name: %s, Tile Width: %i, Tile Height: %i, \n Spacing: %i, Margin: %i, TileCount: %i, Source:%s \n", mapVars.tileVars[i].firstgid, mapVars.tileVars[i].name, mapVars.tileVars[i].tilewidth, mapVars.tileVars[i].tileheight, 
				mapVars.tileVars[i].spacing, mapVars.tileVars[i].margin, mapVars.tileVars[i].tileCount, mapVars.tileVars[i].source);
		}
	
	map_loaded = ret;

	return ret;
}
