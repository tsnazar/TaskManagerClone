#include "LinuxSysInfoImpl.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

#include <unistd.h>

void LinuxSysInfoImpl::ScanSysInfo()
{
	struct sysinfo MemInfo;
	sysinfo(&MemInfo);

	SysInfo.TotalVirtual = MemInfo.totalram;
	SysInfo.TotalVirtual += MemInfo.totalswap;
	SysInfo.TotalVirtual /= 1024 * 1024;

	SysInfo.UsedVirtual = MemInfo.totalram - MemInfo.freeram;
	SysInfo.UsedVirtual = MemInfo.totalswap - MemInfo.freeswap;
	SysInfo.UsedVirtual /= 1024 * 1024;

	SysInfo.TotalPhys = MemInfo.totalram;
	SysInfo.TotalPhys /= 1024 * 1024;

	SysInfo.UsedPhys = MemInfo.totalram - MemInfo.freeram;
	SysInfo.UsedPhys /= 1024 * 1024;

	SysInfo.PageSize = sysconf(_SC_PAGESIZE);
}