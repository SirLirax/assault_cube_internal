#pragma once
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl2.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Menu {
public:
    bool visible;
    Menu();
    ~Menu();

    // Create the ImGui context and set configuration flags.
    void Create();

    // Initialize the backends and apply styles.
    void Initialize();

    // Render the menu UI.
    void renderMenu();
    void render();
};