#pragma once 

#include "../ProcessesImpl.h"

#include <Windows.h>

class WindowsProcessesImpl : public ProcessesImpl
{
public:
	WindowsProcessesImpl();
	virtual ~WindowsProcessesImpl() = default;

	virtual int Scan();

private:
	void TryAdjustPriviliges();

	void ScanProcesses();

	void GetProcessInfo(DWORD PId);
};