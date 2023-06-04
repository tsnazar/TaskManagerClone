#pragma once 

class SysInfoImpl;

class SysInfoFactory
{
public:
	static SysInfoFactory* GetSysInfoFactory();

	virtual SysInfoImpl* MakeSysInfo() = 0;
};