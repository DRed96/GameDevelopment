#include "j1FileSystem.h"

FileSystem::FileSystem()
{
	PHYSFS_init(NULL);
}

FileSystem::~FileSystem()
{
	
}

bool FileSystem::CleanUp()
{
	bool ret = true;
	if (fileList.start != NULL)
		//Iterar la llista d'arxius oberts per tancar-los
		if (PHYSFS_close(fileList.start->data) != 0)
			LOG("PHYSFS_close ERROR: %s", PHYSFS_getLastError());
		//DeInit
		if (PHYSFS_deinit() != 0)
			LOG("PHYSFS_deinit ERROR: %s", PHYSFS_getLastError());
	return ret;
}

bool FileSystem::LoadFile(char*)
{
	PHYSFS_File * toFill = NULL;
}