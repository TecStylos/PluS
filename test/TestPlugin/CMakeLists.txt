cmake_minimum_required(VERSION 3.8)

project("TestPlugin")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(
	TestPlugin SHARED 
	"TestPlugin.cpp"
)

target_include_directories(
	TestPlugin PUBLIC 
	"../../PluS/include"
)

if (WIN32)
   target_compile_definitions(
	TestPlugin PUBLIC
	"PLUS_PLATFORM_WINDOWS"
   )
endif()

if (UNIX)
   target_compile_definitions(
	TestPlugin PUBLIC
	"PLUS_PLATFORM_UNIX"
   )
   target_link_libraries(
	TestPlugin PUBLIC
	dl
   )
endif()
