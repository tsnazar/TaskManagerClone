#include "WindowsSysInfoImpl.h"

#include <Windows.h>

void WindowsSysInfoImpl::ScanSysInfo()
{
	MEMORYSTATUSEX MemInfo;
	MemInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&MemInfo);

	SYSTEM_INFO LocalSysInfo;
	GetSystemInfo(&LocalSysInfo);

	SysInfo.TotalVirtual = MemInfo.ullTotalPageFile;
	SysInfo.TotalVirtual /= 1024 * 1024;

	SysInfo.UsedVirtual = MemInfo.ullTotalPageFile - MemInfo.ullAvailPageFile;
	SysInfo.UsedVirtual /= 1024 * 1024;

	SysInfo.TotalPhys = MemInfo.ullTotalPhys;
	SysInfo.TotalPhys /= 1024 * 1024;

	SysInfo.UsedPhys = MemInfo.ullTotalPhys - MemInfo.ullAvailPhys;
	SysInfo.UsedPhys /= 1024 * 1024;

	SysInfo.PageSize = LocalSysInfo.dwPageSize;
}