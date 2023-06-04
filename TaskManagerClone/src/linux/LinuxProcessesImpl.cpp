#include "LinuxProcessesImpl.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <pwd.h>

#include <fstream>
#include <sstream>
#include <algorithm>

#include "../RabinKarpAlg.h"

int LinuxProcessesImpl::Scan()
{
	//ProcessMap.clear();

	DIR* ProcDir;

	ProcDir = opendir("/proc");
	if (ProcDir == NULL)
		return 0;
	// Read all Proc contents, and record all that 1)
	// are numeric in name, 2) are themselves
	// directories, and 3) contain a file named
	// cmdline. The directory name is the PID, and
	// the contents of cmdline is the command. This
	// has to be slower than using sysctl(3) in BSD,
	// and a bit more clunky, but it is the Linux way
	// of getting process info.
	std::string Name;
	struct dirent* DirEnt;
	struct stat StatBuf;
	while ((DirEnt = readdir(ProcDir))) 
	{
		Name = "/proc/";
		Name += DirEnt->d_name;
		if (!stat(Name.c_str(), &StatBuf)) 
		{
			if (StatBuf.st_mode & S_IFDIR) 
			{
				char* p;
				p = DirEnt->d_name;
				bool allDigits = true;
				while (*p) 
				{
					if (!isdigit(*p)) 
					{
						allDigits = false;
						break;
					}
					p++;
				}
				
				if (allDigits) 
				{
					
					ProcessInfo Proc;
					Proc.Id = atoi(DirEnt->d_name);

					ProcessInfo PrevProc;
					auto ProcIter = ProcessMap.find(Proc.Id);
					const bool ProcWasRunning = ProcIter != ProcessMap.end();
					if (ProcWasRunning)
						PrevProc = ProcIter->second;

					std::string Path = Name + std::string("/cmdline");
					std::ifstream CmdLine(Path);
					
					if (CmdLine.good())
					{
						CmdLine >> Proc.Name;
					}
					else
					{
						continue;
					}

					std::string StatPath = Name + std::string("/stat");
					std::ifstream StatFile(StatPath);
					if (StatFile.good())
					{
						StatFile >> Proc.Id;

						{
							std::streampos CurPos = StatFile.tellg();
							
							std::stringstream Buffer;
							Buffer << StatFile.rdbuf();

							std::string Info = Buffer.str();

							std::string NameSubStr = Info.substr(0, 20);
							std::string ReverseNameSubStr = NameSubStr;
							std::reverse(ReverseNameSubStr.begin(), ReverseNameSubStr.end());

							int LeftBracketIndex = RabinKarp("(", NameSubStr);
							int RightBracketIndex = ReverseNameSubStr.size() - RabinKarp(")", ReverseNameSubStr);

							StatFile.seekg(CurPos);
							StatFile.seekg(RightBracketIndex + 1, std::ios::cur);
							StatFile.clear();

							if(Proc.Name.size() == 0)
								Proc.Name = NameSubStr.substr(LeftBracketIndex + 1, RightBracketIndex - 1);
						}

						StatFile >> Proc.Status;

						unsigned int  Uid;
						StatFile >> Uid;

						struct passwd* Pws = getpwuid(Uid);
						if (Pws)
							Proc.UserName = std::string(getpwuid(Uid)->pw_name);
						else
							Proc.UserName = std::string("Unknown");

						for (int i = 0; i < 9; ++i)
						{
							unsigned int Tmp;
							StatFile >> Tmp;
						}

						StatFile >> Proc.UserCPUTime;
						StatFile >> Proc.SysCPUTime;

						for (int i = 0; i < 8; ++i)
						{
							unsigned int Tmp;
							StatFile >> Tmp;
						}

						unsigned int RSS = 0;
						StatFile >> RSS;
						Proc.MemorySize = RSS / 1024.0;

						Proc.CPUTime = times(nullptr);

						StatFile.close();

						if (ProcWasRunning)
						{
							if (Proc.CPUTime <= PrevProc.CPUTime || Proc.SysCPUTime < PrevProc.SysCPUTime ||
								Proc.UserCPUTime < PrevProc.UserCPUTime) {
								//Overflow detection. Just skip this value.
								Proc.CpuPercentage = -1.0;
							}
							else 
							{
								Proc.CpuPercentage = (Proc.SysCPUTime - PrevProc.SysCPUTime) +
									(Proc.UserCPUTime - PrevProc.UserCPUTime);
								Proc.CpuPercentage /= (Proc.CPUTime - PrevProc.CPUTime);
								Proc.CpuPercentage *= 100;
							}
						}
					}

					ProcessMap.insert_or_assign(Proc.Id, Proc);
				}
			}
		}
	}
	closedir(ProcDir);

	return ProcessMap.size();
}

