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

struct ProcessInfo
{
	unsigned int Id;
	std::string Status;
	double CpuPercentage;
	double MemorySize;
	std::string Name;
	std::string UserName;
	
	PITIME CPUTime;
	PITIME SysCPUTime;
	PITIME UserCPUTime;
};

template<bool SortOrderDescending>
inline bool SortById(const ProcessInfo* lhs, const ProcessInfo* rhs)
{
	if constexpr (SortOrderDescending)
		return lhs->Id < rhs->Id;
	else
		return lhs->Id > rhs->Id;
}
template<bool SortOrderDescending>
inline bool SortByStatus(const ProcessInfo* lhs, const ProcessInfo* rhs)
{
	if constexpr (SortOrderDescending)
		return lhs->Status < rhs->Status;
	else
		return lhs->Status > rhs->Status;
}
template<bool SortOrderDescending>
inline bool SortByCpuPercentage(const ProcessInfo* lhs, const ProcessInfo* rhs)
{
	if constexpr (SortOrderDescending)
		return lhs->CpuPercentage < rhs->CpuPercentage;
	else
		return lhs->CpuPercentage > rhs->CpuPercentage;
}
template<bool SortOrderDescending>
inline bool SortByMemoryPercentage(const ProcessInfo* lhs, const ProcessInfo* rhs)
{
	if constexpr (SortOrderDescending)
		return lhs->MemorySize < rhs->MemorySize;
	else
		return lhs->MemorySize > rhs->MemorySize;
}
template<bool SortOrderDescending>
inline bool SortByName(const ProcessInfo* lhs, const ProcessInfo* rhs)
{
	if constexpr (SortOrderDescending)
		return lhs->Name < rhs->Name;
	else
		return lhs->Name > rhs->Name;
}
template<bool SortOrderDescending>
inline bool SortByUserName(const ProcessInfo* lhs, const ProcessInfo* rhs)
{
	if constexpr (SortOrderDescending)
		return lhs->UserName < rhs->UserName;
	else
		return lhs->UserName > rhs->UserName;
}
