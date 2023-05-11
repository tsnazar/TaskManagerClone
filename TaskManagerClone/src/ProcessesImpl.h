#pragma once

#include "ProcessInfo.h"
#include <unordered_map>

class ProcessesImpl
{
public:
	ProcessesImpl() = default;
	virtual ~ProcessesImpl() = default;
	
	virtual int Scan() = 0;
	virtual void ScanSysInfo() = 0;

	int GetCount() { return ProcessMap.size(); };
	
	GlobalSysInfo GetSysInfo() { return SysInfo; }
	
	ProcessInfo GetProcessInfo(const int PId) { return ProcessMap[PId]; }
	
	std::unordered_map<unsigned int, ProcessInfo>& GetMap() { return ProcessMap; }

protected:
	GlobalSysInfo SysInfo;
	std::unordered_map<unsigned int, ProcessInfo> ProcessMap;
};