#include "systemclass.h"

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	SystemClass* System;
	bool result;

	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}