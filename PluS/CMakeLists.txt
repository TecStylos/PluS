cmake_minimum_required(VERSION 3.8)

project("PluS")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(
	PluS_SOURCES
	"src/PluS.cpp"
	"src/Types.cpp"
	"src/FeatureIterator.cpp"
	"src/PluginFeatureIterator.cpp"
	"src/PluginHandle.cpp"
	"src/PluginHandleCreator.cpp"
	"src/PluginImplementation.cpp"
	"src/PluginManager.cpp"
)

if (WIN32)
	set(
		PluS_SOURCES
		${PluS_SOURCES}
		"src/Platform/Win32PluginHandle.cpp"
	)
elseif (UNIX)
	set(
		PluS_SOURCES
		${PluS_SOURCES}
		"src/Platform/UnixPluginHandle.cpp"
	)
endif()

add_library(
	PluS SHARED
	${PluS_SOURCES}
)

SET_TARGET_PROPERTIES(
	PluS PROPERTIES PREFIX ""
)

target_include_directories(
	PluS PUBLIC
	"include"
)

if (WIN32)
	target_compile_definitions(
		PluS PRIVATE
		"PLUS_PLATFORM_WINDOWS"
		"PLUS_BUILD_LIBRARY"
	)
endif()

if (UNIX)
	target_compile_definitions(
		PluS PRIVATE
		"PLUS_PLATFORM_UNIX"
		"PLUS_BUILD_LIBRARY"
	)
	target_link_libraries(
		PluS PUBLIC
		dl
	)
endif()
