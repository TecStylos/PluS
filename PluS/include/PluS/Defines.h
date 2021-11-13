#pragma once

#ifdef PLUS_PLATFORM_UNIX
#define __cdecl
//#define __cdecl __attribute((__cdecl__))
#endif

#if defined PLUS_PLATFORM_WINDOWS
  #define PLUS_PROCESS_ID unsigned long
  #define PLUS_PLATFORM_PLUGIN_EXTENSION ".dll"
  #if defined PLUS_BUILD_LIBRARY
    #define PLUS_API __declspec(dllexport)
  #else
    #define PLUS_API __declspec(dllimport)
  #endif
#elif defined PLUS_PLATFORM_UNIX
  #define PLUS_PROCESS_ID int
  #define PLUS_PLATFORM_PLUGIN_EXTENSION ".so"
  #if defined PLUS_BUILD_LIBRARY
    #define PLUS_API __attribute__((visibility("default")))
  #else
    #define PLUS_API
  #endif
#else
  #error PLUS_PLATFORM_<PLATFORM> NOT DEFINED!
#endif
