cmake_minimum_required(VERSION 3.1)
project(dsapiforwardedfor)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

include_directories(thirdparty/notesapi)
if(UNIX)
	# Assume it's Linux
	add_definitions("-DGCC3 -DGCC4 -fno-strict-aliasing -DGCC_LBLB_NOT_SUPPORTED -Wformat -Wall -Wcast-align  -DUNIX -DLINUX -DLINUX86 -DND64 -DW32 -DLINUX64 -DW -DLINUX86_64 -DDTRACE -DPTHREAD_KERNEL -D_REENTRANT -DUSE_THREADSAFE_INTERFACES -D_POSIX_THREAD_SAFE_FUNCTIONS  -DHANDLE_IS_32BITS -DHAS_IOCP -DHAS_BOOL -DHAS_DLOPEN -DUSE_PTHREAD_INTERFACES -DLARGE64_FILES -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -DNDUNIX64 -DLONGIS64BIT   -DPRODUCTION_VERSION -DOVERRIDEDEBUG -fPIC")
	
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -std=c++11 -static-libgcc")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -m64 -std=c++11 -static-libgcc")
	set (CMAKE_SHARED_LINKER_FLAGS "-lnotes -lpthread -lm -lnsl -lresolv -dl")
else()
	message (STATUS "Could not determine build type")
endif()

set(SOURCE_FILES
	src/dsapiforwardedfor.c
)

add_library(dsapiforwardedfor SHARED ${SOURCE_FILES})
target_link_libraries(dsapiforwardedfor notes)