if(WIN32)
target_sources(TaskManagerClone PRIVATE "windows/WindowsProcessesImpl.cpp" "windows/WindowsSysInfoImpl.cpp")
endif(WIN32)
