#ifndef _F_SYSTEM_H_
#define _F_SYSTEM_H_

#include "j1Module.h"
#include "PhysFS\include\physfs.h"
#include "p2List.h"
#include "p2Log.h"

//#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment(lib, "PhysFS/libx86/physfs.lib")
class FileSystem: public j1Module
{
public:
	FileSystem();
	~FileSystem();
	bool CleanUp();
	bool  LoadFile(char*);
private:
	//Llista d'arxius oberts
	p2List<PHYSFS_File *> fileList;
};




#endif // !_F_SYSTEM_H_
