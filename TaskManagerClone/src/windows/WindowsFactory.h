#pragma once 

#include "../ProcessesFactory.h"
#include "../ProcessesImpl.h"
#include "../SysInfoFactory.h"
#include "../SysInfoImpl.h"

#include "WindowsProcessesImpl.h"
#include "WindowsSysInfoImpl.h"

class WindowsFactory : public ProcessesFactory, public SysInfoFactory
{
public:
	static WindowsFactory* GetFactoryInstance()
	{
		static WindowsFactory* WinFactory = nullptr;
		if (!WinFactory)
		{
			WinFactory = new WindowsFactory;
		}
		return WinFactory;
	}

	virtual ~WindowsFactory() = default;
	
	virtual ProcessesImpl* MakeProcesses() final
	{
		return new WindowsProcessesImpl;
	};

	virtual SysInfoImpl* MakeSysInfo() final
	{
		return new WindowsSysInfoImpl;
	};

private:
	WindowsFactory() = default;
};