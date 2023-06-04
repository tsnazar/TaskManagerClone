#include "SysInfoFactory.h"

#ifdef WIN_SYS
#include "windows/WindowsFactory.h"
#endif

#ifdef UNIX_SYS
#include "linux/LinuxFactory.h"
#endif 

SysInfoFactory* SysInfoFactory::GetSysInfoFactory()
{
	static SysInfoFactory* Factory = nullptr;

	if (!Factory)
	{
#ifdef WIN_SYS
		Factory = WindowsFactory::GetFactoryInstance();
#endif

#ifdef UNIX_SYS
		Factory = LinuxFactory::GetFactoryInstance();
#endif
	}

	return Factory;
}