#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct tileSet_variables
{
	int firstgid;
	char * source;
	char * name;
	int tilewidth;
	int tileheight;
	int spacing;
	int margin;
	int tileCount;
	int tileOffset_x;
	int tileOffset_y;
};

// TODO 1: Create a struct needed to hold the information to Map node
enum mapOrientation
{
	Orthogonal,
	Isometric,
	Straggered 
	
};

enum renderOrder
{
	Right_down,
	Right_up,
	Left_down,
	Left_up
};


struct map_variables
{
	float map_version;
	mapOrientation orientation;
	renderOrder render_order;
	int width;
	int height ;
	int tileWidth ;
	int tileHeight;
	int nextObjectId;
	float backgroundColor;
	p2List<tileSet_variables> tileVars;
};
// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:
	map_variables loadMapHeader()
	{
		mapVars->map_version = map_file.child("map").attribute("version").as_float();
		
		mapVars->width = map_file.child("map").attribute("width").as_int();
		mapVars->height = map_file.child("map").attribute("height").as_int();
		mapVars->tileWidth = map_file.child("map").attribute("tilewidth").as_int();
		mapVars->tileHeight = map_file.child("map").attribute("tileheight").as_int();
		mapVars->nextObjectId = map_file.child("map").attribute("nextobjectid").as_int();

		if (map_file.child("map").attribute("orientation").value() == "orthogonal")
			mapVars->orientation = Orthogonal;
		else if (map_file.child("map").attribute("orientation").value() == "isometric")
			mapVars->orientation = Isometric;
		else if (map_file.child("map").attribute("orientation").value() == "straggered")
			mapVars->orientation = Straggered;

		if (map_file.child("map").attribute("renderorder").value() == "right-down")
			mapVars->render_order = Right_down;
		else if (map_file.child("map").attribute("renderorder").value() == "right-up")
			mapVars->render_order = Right_up;
		else if (map_file.child("map").attribute("renderorder").value() == "left-down")
			mapVars->render_order = Left_down;
		else if (map_file.child("map").attribute("renderorder").value() == "left-up")
			mapVars->render_order = Left_up;
	}
public:

	// TODO 1: Add your struct for map info as public for now
	map_variables* mapVars;
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__