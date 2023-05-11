add_subdirectory("libs/wxWidgets")
add_subdirectory("src") 
if(WIN32)
add_subdirectory("src/windows") 
endif(WIN32)

if(UNIX)
add_subdirectory("src/linux") 
endif(UNIX)
