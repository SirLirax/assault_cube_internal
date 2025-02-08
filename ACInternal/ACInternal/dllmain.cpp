// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"
#include <iostream>

#define local_player_pointer  0x17E0A8
#define local_player_pointer1 0x18AC00
#define local_player_pointer2 0x195404
#define player_count_pointer 0x18AC0C
#define entity_list_pointer 0x18AC04
#define player_viewmatrix 0x17DFD0

void initialzeConsole()
{
  AllocConsole();
  FILE* f;
  freopen_s(&f, "CONOUT$", "w", stdout);
}

HMODULE getModuleBaseAddress(LPCWSTR executeable_name)
{
  std::cout << "[~] Getting module base address.";
  HMODULE module_base_address = GetModuleHandle(executeable_name);
  if (module_base_address)
  {
    std::cout << "[+] Found module base at: 0x" << std::hex << module_base_address;
    return module_base_address;
  }
  std::cout << "[-] Couldnt get module base address.";
  return nullptr;
}

DWORD mainThread()
{
  initialzeConsole();
  std::cout << "[+] Console initialized.";
  HANDLE module_base_address = getModuleBaseAddress(L"ac_client.exe");
  return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
  HANDLE file_handle = nullptr;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    file_handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, nullptr);
    case DLL_PROCESS_DETACH:
      if(file_handle)
      {
        CloseHandle(file_handle);
      }
        break;
    }
    return TRUE;
}

