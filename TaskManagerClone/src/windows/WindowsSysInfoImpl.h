#pragma once

#include "../SysInfoImpl.h"

class WindowsSysInfoImpl : public SysInfoImpl
{
public:
	WindowsSysInfoImpl() = default;
	virtual ~WindowsSysInfoImpl() = default;

	virtual void ScanSysInfo();
};