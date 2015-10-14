#include "j1Physlib.h"
#include "p2Log.h"


j1Physlib::j1Physlib() : j1Module(){}
j1Physlib::~j1Physlib(){}

bool j1Physlib::Awake()
{
	bool status = true;
	LOG("Initializing");
	if (PHYSFS_init(NULL) == 0)
	{
		status = false;
		LOG("Failed to initialize ERROR:%s\n", PHYSFS_getLastError());
	}

	return status;
}

bool j1Physlib::CleanUp()
{
	bool status = true;
	if (PHYSFS_deinit() == 0)
	{
		status = false;
		LOG("Failed to cleanup. ERROR:%s\n", PHYSFS_getLastError());
	}
		
	return status;
}