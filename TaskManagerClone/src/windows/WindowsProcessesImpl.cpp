#include "WindowsProcessesImpl.h"
#include "WindowsProcessesImpl.h"

WindowsProcessesImpl::WindowsProcessesImpl()
{
	// set SE_DEBUG_NAME to view more processes, program must be started with admin rights
	TryAdjustPriviliges();
}

int WindowsProcessesImpl::Scan()
{
	ScanProcesses();

	return ProcessMap.size();
}

void WindowsProcessesImpl::ScanSysInfo()
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

void WindowsProcessesImpl::TryAdjustPriviliges()
{
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return;

	LUID Luid;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid))
	{
		CloseHandle(hToken);
		return;
	}

	TOKEN_PRIVILEGES TokenPriv;
	TokenPriv.PrivilegeCount = 1;
	TokenPriv.Privileges[0].Luid = Luid;
	TokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &TokenPriv, sizeof(TokenPriv), NULL, 0))
	{
		CloseHandle(hToken);
		return;
	}

	CloseHandle(hToken);
}

void WindowsProcessesImpl::ScanProcesses()
{
	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses),
		&cbNeeded))
		return;
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);
	// Print the name and process identifier for each process.
	for (i = 0; i < cProcesses; i++)
		if (aProcesses[i] != 0)
			GetProcessInfo(aProcesses[i]);
};

void WindowsProcessesImpl::GetProcessInfo(DWORD PId)
{
	TCHAR ProcessName[MAX_PATH] = TEXT("<unknown>");
	TCHAR UserName[MAX_PATH] = TEXT("<unknown>");
	TCHAR DomainName[MAX_PATH] = TEXT("<unknown>");
	DWORD StatusCode = -1;

	ProcessInfo PrevProc, Proc;
	auto ProcIter = ProcessMap.find(PId);
	const bool ProcWasRunning = ProcIter != ProcessMap.end();
	if (ProcWasRunning)
		PrevProc = ProcIter->second;

	// Get a handle to the process.
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PId);
	// Get the process name.
	if (hProcess != NULL)
	{
		if (!GetExitCodeProcess(hProcess, &StatusCode))
		{
			if (ProcWasRunning)
				ProcessMap.erase(PId);
			CloseHandle(hProcess);
			return;
		}

		HMODULE hMod;
		DWORD CbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &CbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, ProcessName, sizeof(ProcessName) / sizeof(TCHAR));
		}

		HANDLE hToken;
		if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		{
			DWORD tokenSize = 0;
			GetTokenInformation(hToken, TokenUser, NULL, 0, &tokenSize);
			if (tokenSize > 0)
			{
				BYTE* data = new BYTE[tokenSize];
				GetTokenInformation(hToken, TokenUser, data, tokenSize, &tokenSize);
				
				TOKEN_USER* pUser = (TOKEN_USER*)data;
				PSID pSID = pUser->User.Sid;
				
				SID_NAME_USE SidType;
				DWORD UserNameLen = sizeof(UserName) / sizeof(TCHAR);
				DWORD DomainNameLen = sizeof(DomainName) / sizeof(TCHAR);
				LookupAccountSid(NULL, pSID, UserName, &UserNameLen, DomainName, &DomainNameLen, &SidType);
				
				delete[] data;
			}
		}
		CloseHandle(hToken);
		
		{
			PROCESS_MEMORY_COUNTERS_EX Pmc;
			GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&Pmc, sizeof(Pmc));
			Proc.MemorySize =  Pmc.WorkingSetSize / (1024.0 * 1024.0);
		}

		if (ProcWasRunning)
		{
			FILETIME ftime, fsys, fuser;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&Proc.CPUTime, &ftime, sizeof(FILETIME));

			GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
			memcpy(&Proc.SysCPUTime, &fsys, sizeof(FILETIME));
			memcpy(&Proc.UserCPUTime, &fuser, sizeof(FILETIME));

			Proc.CpuPercentage = (Proc.SysCPUTime.QuadPart - PrevProc.SysCPUTime.QuadPart) +
				(Proc.UserCPUTime.QuadPart - PrevProc.UserCPUTime.QuadPart);
			Proc.CpuPercentage /= (Proc.CPUTime.QuadPart - PrevProc.CPUTime.QuadPart);
			Proc.CpuPercentage *= 100.0;
		}
		else
		{
			FILETIME ftime, fsys, fuser;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&Proc.CPUTime, &ftime, sizeof(FILETIME));

			GetProcessTimes(hProcess, &ftime, &ftime, &fsys, &fuser);
			memcpy(&Proc.SysCPUTime, &fsys, sizeof(FILETIME));
			memcpy(&Proc.UserCPUTime, &fuser, sizeof(FILETIME));
		}
	}
	else
	{
		return;
	}

	Proc.Id = PId;
	Proc.Status = StatusCode != STILL_ACTIVE ? "U" : "R";
	Proc.Name = std::string(ProcessName);
	Proc.UserName = std::string(UserName);

	ProcessMap.insert_or_assign(Proc.Id, Proc);
	
	CloseHandle(hProcess);
}
