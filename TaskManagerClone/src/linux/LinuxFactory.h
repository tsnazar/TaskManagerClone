#pragma once 

#include "../ProcessesFactory.h"
#include "../ProcessesImpl.h"
#include "LinuxProcessesImpl.h"

class LinuxFactory : public ProcessesFactory
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
private:
	LinuxFactory() = default;
};