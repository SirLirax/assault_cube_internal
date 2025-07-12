// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <gl/GL.h>
#include "mem.h"
#include "hook.h"
#include "menu.h"
#include "hack.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl2.h"

bool g_show_menu = true;

enum SDL_bool { SDL_FALSE = 0, SDL_TRUE = 1 };
typedef int(__cdecl* tSDL_SetRelativeMouseMode)(SDL_bool enabled);
static tSDL_SetRelativeMouseMode oSDL_SetRelativeMouseMode = nullptr;

int __cdecl hkSDL_SetRelativeMouseMode(SDL_bool enabled)
{
	if (g_show_menu)
	{
		if (enabled == SDL_TRUE)
			return 0;
		else
			return oSDL_SetRelativeMouseMode(enabled);
	}
	else
	{
		return oSDL_SetRelativeMouseMode(enabled);
	}
}

typedef BOOL(__stdcall* tWGL_SwapBuffers) (HDC hDc);
static tWGL_SwapBuffers oWGL_SwapBuffersGateway = nullptr;

BOOL __stdcall hkWGL_SwapBuffers(HDC hDc)
{
	static bool menuCreated = false;
	static Menu menu;
	if (!menuCreated) {
		menu.Create();
		menu.Initialize();
		menuCreated = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		bool wasOpen = menu.visible;
		menu.visible = !menu.visible;
		g_show_menu = menu.visible;

		if (!wasOpen && menu.visible)
		{
			if (oSDL_SetRelativeMouseMode)
			{
				oSDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
		else if (wasOpen && !menu.visible)
		{
			if (oSDL_SetRelativeMouseMode)
			{
				oSDL_SetRelativeMouseMode(SDL_TRUE);
			}
		}
	}
	// Begin a new ImGui frame.
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Render the menu.
	menu.renderMenu();
	RunHack(); // This calls your Esp() among others.

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	return oWGL_SwapBuffersGateway(hDc);
}


void initialzeConsole(FILE** f)
{
	AllocConsole();
	freopen_s(f, "CONOUT$", "w", stdout);
}

HMODULE getModuleBaseAddress(LPCWSTR executeable_name)
{
	std::cout << "[~] Getting module base address.\n";
	HMODULE module_base_address = GetModuleHandle(executeable_name);
	if (module_base_address)
	{
		std::cout << "[+] Found module base at: 0x" << std::hex << module_base_address << "\n";
		return module_base_address;
	}
	std::cout << "[-] Couldnt get module base address.\n";
	return nullptr;
}

void initalizeHooks()
{
	std::cout << "[~] Initializing wglSwapBuffers hook.\n";
	Hook wglSwapBufferHook((BYTE*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"), (BYTE*)hkWGL_SwapBuffers, (BYTE*)&oWGL_SwapBuffersGateway, 5);
	if (!oWGL_SwapBuffersGateway)
	{
		std::cout << "[-] Hooking wglSwapBuffers failed.\n";
	}


	std::cout << "[~] Initializing SDL_SetRelativeMouseMode hook.\n";
	Hook sdlHook((BYTE*)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode"), (BYTE*)hkSDL_SetRelativeMouseMode, (BYTE*)&oSDL_SetRelativeMouseMode, 7);
	if (!oWGL_SwapBuffersGateway)
	{
		std::cout << "[-] Hooking SDL_SetRelativeMouseMode failed.\n";
	}

	std::cout << "[~] Enabling hooks.\n";
	wglSwapBufferHook.enableHook();
	sdlHook.enableHook();
	std::cout << "[+] Hooks enabled.\n";
}

DWORD mainThread()
{
	FILE* file;
	initialzeConsole(&file);
	std::cout << "[+] Console initialized.\n";
	HANDLE module_base_address = getModuleBaseAddress(L"ac_client.exe");
	initalizeHooks();
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)UpdateEntityList, nullptr, 0, nullptr);
	std::cout << "[*] Press END to exit.\n";
	while (!GetAsyncKeyState(VK_END))
	{
		Sleep(100);
	}
	fclose(file);
	FreeConsole();
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
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
		if (file_handle)
		{
			CloseHandle(file_handle);
		}
		break;
	}
	return TRUE;
}

