#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2DynArray.h"
// TODO 1: Create a struct for the map layer
// TODO 6: Inside struct for the layer, create a short method to get the value of x,y
// ----------------------------------------------------
struct Layer
{
	const char * name;
	unsigned int width;
	unsigned int height;
	//p2DynArray<unsigned int> data;
	unsigned int* data;
	Layer() : name(NULL), width(0), height(0), data(NULL)
	{}
	~Layer()
	{
		if (name != NULL)
			delete [] (name);
		if (data != NULL)
			delete [] (data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(x + y*width)];
	}
};
// ----------------------------------------------------
struct TileSet
{
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const
	{
		
			/*SDL_Rect ret;
		float tmp_x = id;
		float tmp_x2 = num_tiles_width;
		ret.w = tile_width;
		ret.h = tile_height;
		//Determinar la fila
		ret.y = gid / num_tiles_width * tile_height +spacing + margin;
		//Determinar la columna
		if (id % num_tiles_width != 0)
			ret.x = ((gid % num_tiles_width) - 1)* tile_width + 5;
		else
			ret.x = 8 * tile_width;
		//ret.x-=
	
		//ret.x = () * num_tiles_width * tile_width;
		return ret;*/
		uint gid = id - firstgid;
		int tile_x;
		int tile_y;
		if(gid >= num_tiles_width)
		{
			tile_x = gid % num_tiles_width;
			tile_y = (gid - tile_x) / num_tiles_width;
		}
		else
		{
			tile_x = gid;
			tile_y = 0;
		}
		//LOG("tileID = %d, gid = %d, tile x = %d, tile y = %d", tileID, gid, tile_x, tile_y);
		//pixel position
		int x = tile_x*(tile_width + margin) + margin;
		int y = tile_y*(tile_height + spacing) + spacing;

		return{ x, y, tile_width, tile_height };
	}

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List<Layer*> layers;
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

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single layer
	bool LoadLayer(pugi::xml_node& node, Layer* layers);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__