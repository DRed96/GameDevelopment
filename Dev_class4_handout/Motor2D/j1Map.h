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
	Isometric_straggered,
	Hexagonal_straggered
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
	int tilewidth ;
	int tileheight;
	int nextobjectid;
	float backgroundcolor;
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

public:

	// TODO 1: Add your struct for map info as public for now
	map_variables mapVars;
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__