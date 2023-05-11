#pragma once 

#include "../ProcessesFactory.h"
#include "../ProcessesImpl.h"
#include "WindowsProcessesImpl.h"

class WindowsFactory : public ProcessesFactory
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
private:
	WindowsFactory() = default;
};