// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <gl/GL.h>
#include "mem.h"
#include "hook.h"
#include "C:\Users\laure\Desktop\assaul_cube\assault_cube_internal\ACInternal\imgui\imgui.h"
#include "C:\Users\laure\Desktop\assaul_cube\assault_cube_internal\ACInternal\imgui\imgui_impl_win32.h"
#include "C:\Users\laure\Desktop\assaul_cube\assault_cube_internal\ACInternal\imgui\imgui_impl_opengl2.h"

#define local_player_pointer  0x17E0A8
#define local_player_pointer1 0x18AC00
#define local_player_pointer2 0x195404
#define player_count_pointer 0x18AC0C
#define entity_list_pointer 0x18AC04
#define player_viewmatrix 0x17DFD0

extern HWND g_hWnd = nullptr;

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers wglSwapBuffersGateway;

BOOL __stdcall hookWglSwapBuffers(HDC hDc)
{
	// Ensure we have a valid window handle.
	if (g_hWnd == nullptr)
	{
		// Try to get the foreground window (the currently active window).
		g_hWnd = GetForegroundWindow();
		// As a fallback, try GetActiveWindow.
		if (g_hWnd == nullptr)
		{
			g_hWnd = GetActiveWindow();
		}
	}

	static bool g_ImGuiInitialized = false;
	if (!g_ImGuiInitialized)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		if (!ImGui_ImplWin32_Init(g_hWnd))
		{
			MessageBoxA(NULL, "ImGui_ImplWin32_Init failed!", "Error", MB_OK);
		}
		
		if (!ImGui_ImplOpenGL2_Init())
		{
			MessageBoxA(NULL, "ImGui_ImplOpenGL2_Init failed!", "Error", MB_OK);
		}

		g_ImGuiInitialized = true;
	}

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Assault Cube Overlay");
	ImGui::Text("Hello from ImGui in Assault Cube (OpenGL2)!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return wglSwapBuffersGateway(hDc);
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

twglSwapBuffers initializeHook()
{
	printf("[~] Initializing hook.\n");
	twglSwapBuffers originalSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
	twglSwapBuffers trampolinePointer = (twglSwapBuffers)TrampHook32((BYTE*)originalSwapBuffers, (BYTE*)hookWglSwapBuffers, 5);
	if (trampolinePointer == nullptr)
	{
		printf("[-] Hooking failed.\n");
		return nullptr;
	}
	printf("[+] Hooked wglSwapbuffers\n");
	return trampolinePointer;
}

DWORD mainThread()
{
	FILE* file;
	initialzeConsole(&file);
	std::cout << "[+] Console initialized.\n";
	HANDLE module_base_address = getModuleBaseAddress(L"ac_client.exe");
	twglSwapBuffers trampoline = initializeHook();
	if (trampoline != nullptr)
	{
		wglSwapBuffersGateway = trampoline;
	}
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

