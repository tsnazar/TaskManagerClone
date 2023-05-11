#include "ProcessesFactory.h"

#ifdef WIN_SYS
#include "windows/WindowsFactory.h"
#endif

#ifdef UNIX_SYS
#include "linux/LinuxFactory.h"
#endif 

ProcessesFactory* ProcessesFactory::GetProcessesFactory()
{
	static ProcessesFactory* ProcFactory = nullptr;

	if (!ProcFactory)
	{
#ifdef WIN_SYS
		ProcFactory = WindowsFactory::GetFactoryInstance();
#endif

#ifdef UNIX_SYS
		ProcFactory = LinuxFactory::GetFactoryInstance();
#endif
	}

	return ProcFactory;
}