#pragma once 

#include <string>
#include <vector>

#ifdef WIN_SYS
#include <Windows.h>
typedef ULARGE_INTEGER PITIME;
#endif

#ifdef UNIX_SYS
#include "sys/times.h"
typedef int PITIME;
#endif

struct GlobalSysInfo
{
	unsigned long long PageSize;
	unsigned long long TotalPhys;
	unsigned long long UsedPhys;
	unsigned long long TotalVirtual;
	unsigned long long UsedVirtual;
};