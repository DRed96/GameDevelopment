#ifndef _J1_PHYSLIB_H_
#define _J1_PHYSLIB_H_

#include "PhysFS\include\physfs.h"

#include "j1Module.h"

class j1Physlib : public j1Module
{
public:
	j1Physlib();
	~j1Physlib();


	bool Awake();

	bool CleanUp();

private:

};


#endif // !_J1_PHYSLIB_H_
