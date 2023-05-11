#pragma once 

#include "../ProcessesImpl.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

class LinuxProcessesImpl : public ProcessesImpl
{
public:
	LinuxProcessesImpl();
	virtual ~LinuxProcessesImpl() = default;
	virtual int Scan();
	virtual void ScanSysInfo();
};