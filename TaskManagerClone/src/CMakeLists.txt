add_executable (TaskManagerClone "Main.cpp" "ProcessesFactory.cpp" "RabinKarpAlg.cpp" "SysInfoFactory.cpp")



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TaskManagerClone PROPERTY CXX_STANDARD 20)
endif()

if(WIN32)
set_target_properties(TaskManagerClone PROPERTIES WIN32_EXECUTABLE TRUE)
target_compile_definitions(TaskManagerClone PUBLIC WIN_SYS)
endif(WIN32)

if(UNIX)
target_compile_definitions(TaskManagerClone PUBLIC UNIX_SYS)
endif(UNIX)

target_link_libraries(TaskManagerClone wx::net wx::core wx::base)
