#pragma once 

#include "../SysInfoImpl.h"

class LinuxSysInfoImpl : public SysInfoImpl
{
public:
	LinuxSysInfoImpl() = default;
	virtual ~LinuxSysInfoImpl() = default;

	virtual void ScanSysInfo();
};