#pragma once

#include "SysInfo.h"

class SysInfoImpl
{
public:
	SysInfoImpl() = default;
	virtual ~SysInfoImpl() = default;

	virtual void ScanSysInfo() = 0;

	GlobalSysInfo GetSysInfo() { return SysInfo; }

protected:
	GlobalSysInfo SysInfo;
};