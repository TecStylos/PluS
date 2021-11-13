#include <Windows.h>

#include <iostream>

extern "C" __declspec(dllexport) DWORD CallMeIfYouCan(LPVOID data)
{
    std::cout << "You called me because you can!" << std::endl;
    return 1234;
}

BOOL DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpReservd)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
     // Initialize once for each new process.
     // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
     // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
     // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
     // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}