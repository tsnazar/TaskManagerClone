#pragma once

#include "ProcessesFactory.h"
#include "ProcessesImpl.h"

class ProcessList
{
public:
	ProcessList()
		: Impl(nullptr)
	{
		ProcessesFactory* Factory = ProcessesFactory::GetProcessesFactory();

		if (Factory)
			Impl = Factory->MakeProcesses();
	}
	
	int ScanProcesses()
	{
		if (Impl)
			return Impl->Scan();

		return -1;
	};

	int GetCount() { return Impl->GetCount(); };

	ProcessInfo GetProcessInfo(const int PId) { return Impl->GetProcessInfo(PId); };
	
	std::unordered_map<unsigned int, ProcessInfo>& GetMap() { return Impl->GetMap(); };

private:
	ProcessesImpl* Impl;
};