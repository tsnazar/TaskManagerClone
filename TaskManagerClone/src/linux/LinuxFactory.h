#pragma once 

#include "../ProcessesFactory.h"
#include "../ProcessesImpl.h"
#include "../SysInfoFactory.h"
#include "../SysInfoImpl.h"

#include "LinuxProcessesImpl.h"
#include "LinuxSysInfoImpl.h"

class LinuxFactory : public ProcessesFactory, public SysInfoFactory
{
public:
	static LinuxFactory* GetFactoryInstance()
	{
		static LinuxFactory* LinFactory = nullptr;
		if (!LinFactory)
		{
			LinFactory = new LinuxFactory;
		}
		return LinFactory;
	}

	virtual ~LinuxFactory() = default;

	virtual ProcessesImpl* MakeProcesses() final
	{
		return new LinuxProcessesImpl;
	};

	virtual SysInfoImpl* MakeSysInfo() final
	{
		return new LinuxSysInfoImpl;
	};

private:
	LinuxFactory() = default;
};