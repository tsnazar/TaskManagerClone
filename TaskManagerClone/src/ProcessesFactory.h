#pragma once 

class ProcessesImpl;

class ProcessesFactory
{
public:
	static ProcessesFactory* GetProcessesFactory();

	virtual ProcessesImpl* MakeProcesses() = 0;
};