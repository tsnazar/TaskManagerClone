if(UNIX)
target_sources(TaskManagerClone PRIVATE "linux/LinuxProcessesImpl.cpp" "linux/LinuxSysInfoImpl.cpp")
endif(UNIX)
