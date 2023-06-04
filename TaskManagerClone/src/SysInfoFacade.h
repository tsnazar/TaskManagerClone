#pragma once

#include "SysInfoFactory.h"
#include "SysInfoImpl.h"

class SysInfoFacade
{
public:
	SysInfoFacade()
		: Impl(nullptr)
	{
		SysInfoFactory* Factory = SysInfoFactory::GetSysInfoFactory();

		if (Factory)
			Impl = Factory->MakeSysInfo();
	}

	void ScanSysInfo() { Impl->ScanSysInfo(); }

	GlobalSysInfo GetSysInfo() { return Impl->GetSysInfo(); }

private:
	SysInfoImpl* Impl;
};