#pragma once 

#include "../ProcessesImpl.h"

class LinuxProcessesImpl : public ProcessesImpl
{
public:
	LinuxProcessesImpl() = default;
	virtual ~LinuxProcessesImpl() = default;

	virtual int Scan();
};