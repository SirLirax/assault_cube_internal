// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"

#define local_player_pointer  0x17E0A8
#define local_player_pointer1 0x18AC00
#define local_player_pointer2 0x195404



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

