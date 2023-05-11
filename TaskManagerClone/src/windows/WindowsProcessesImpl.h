#pragma once 

#include "../ProcessesImpl.h"

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "psapi.h"

class WindowsProcessesImpl : public ProcessesImpl
{
public:
	WindowsProcessesImpl();
	virtual ~WindowsProcessesImpl() = default;

	virtual int Scan();

	virtual void ScanSysInfo();
private:
	void TryAdjustPriviliges();

	void ScanProcesses();

	void GetProcessInfo(DWORD PId);
};