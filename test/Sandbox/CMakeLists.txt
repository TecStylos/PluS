cmake_minimum_required(VERSION 3.8)

project("Sandbox")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(
	Sandbox 
	"Sandbox.cpp"
)

target_include_directories(
	Sandbox PUBLIC 
	"../../PluS/include"
)

if (WIN32)
   target_compile_definitions(
	Sandbox PUBLIC
	"PLUS_PLATFORM_WINDOWS"
   )
endif()

if (UNIX)
   target_compile_definitions(
	Sandbox PUBLIC
	"PLUS_PLATFORM_UNIX"
   )
   target_link_libraries(
	Sandbox PUBLIC
	dl
   )
endif()
