cmake_minimum_required(VERSION 3.8)

project("PayloadInjector")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(
	PayloadInjector
	"PayloadInjector.cpp"
)

target_include_directories(
	PayloadInjector PUBLIC
	"../../PluS/include"
)

target_link_libraries(
	PayloadInjector PUBLIC
	PluS
)

if (WIN32)
	target_compile_definitions(
		PayloadInjector PUBLIC
		"PLUS_PLATFORM_WINDOWS"
	)
	add_custom_command(
		TARGET PayloadInjector POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
    	"${PROJECT_SOURCE_DIR}/../../out/build/x64-$<CONFIG>/PluS/PluS.dll"
    	$<TARGET_FILE_DIR:PayloadInjector>
	)
endif()