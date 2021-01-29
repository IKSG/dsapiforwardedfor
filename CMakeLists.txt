cmake_minimum_required(VERSION 3.1)
project(dsapiforwardedfor)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

include_directories(thirdparty/notesapi)
if(UNIX)
	# Assume it's Linux
	message (STATUS "Building for Linux64")
	add_definitions("-DUNIX -DLINUX -DND64 -DLINUX64 -DNDUNIX64 -DLONGIS64BIT")
	add_compile_options("-fPIC")
	
	set (CMAKE_SHARED_LINKER_FLAGS "-fPIC")
else()
	message (STATUS "Could not determine build type")
endif()

set(SOURCE_FILES
	src/dsapiforwardedfor.c
)

add_library(dsapiforwardedfor SHARED ${SOURCE_FILES})
target_link_libraries(dsapiforwardedfor notes pthread c m nsl resolv dl)