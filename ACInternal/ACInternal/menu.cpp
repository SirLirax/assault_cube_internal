#include "menu.h"
#include "hack.h"
#include <iostream>

Menu::Menu() : visible(true) {}

Menu::~Menu() {}

static WNDPROC oWndProc = nullptr; 
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true; 

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void Menu::Create()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
}

void Menu::Initialize() {
    HWND hwnd = nullptr;
    while (hwnd == nullptr)
    {
        hwnd = FindWindowA(nullptr, "AssaultCube");
    }

    oWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MyWndProc);

    if (!ImGui_ImplWin32_Init(hwnd)) {
        std::cout << "ImGui_ImplWin32_Init failed!" << std::endl;
    }
    if (!ImGui_ImplOpenGL2_Init()) {
        std::cout << "ImGui_ImplOpenGL2_Init failed!" << std::endl;
    }

    // Get the style so we can modify it
    ImGuiStyle* style = &ImGui::GetStyle();

    // Example: basic spacing & rounding
    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 4);

    // We'll define some colors that approximate Ubuntu's dark theme with orange highlights.
    // Ubuntu's orange is approximately #E95420 => (0.9137, 0.3294, 0.1255)
    ImVec4 ubuntuOrange = ImVec4(0.9137f, 0.3294f, 0.1255f, 1.0f);
    ImVec4 darkGray = ImVec4(0.17f, 0.17f, 0.17f, 1.00f); // #2C2C2C
    ImVec4 lighterGray = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    ImVec4 textWhite = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);

    // Now assign them to the style->Colors array:
    ImVec4* colors = style->Colors;

    style->GrabRounding = 4.0f;

    // --- Override resize grip (expansion corner) appearance ---
    colors[ImGuiCol_ResizeGrip] = darkGray;
    colors[ImGuiCol_ResizeGripHovered] = ubuntuOrange;
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.76f, 0.28f, 0.11f, 1.0f);

    // Backgrounds
    colors[ImGuiCol_WindowBg] = darkGray;
    colors[ImGuiCol_ChildBg] = darkGray;
    colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.94f);

    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.45f, 0.45f, 0.45f, 0.50f);

    // Text
    colors[ImGuiCol_Text] = textWhite;
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Headers
    colors[ImGuiCol_Header] = ubuntuOrange;
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.95f, 0.40f, 0.16f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.28f, 0.11f, 1.0f);

    // Buttons
    colors[ImGuiCol_Button] = lighterGray;
    colors[ImGuiCol_ButtonHovered] = ubuntuOrange;
    colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.28f, 0.11f, 1.0f);

    // Frames
    colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    colors[ImGuiCol_TabHovered] = ubuntuOrange;
    colors[ImGuiCol_TabActive] = ImVec4(0.76f, 0.28f, 0.11f, 1.0f);

    // Sliders
    colors[ImGuiCol_SliderGrab] = ubuntuOrange;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.76f, 0.28f, 0.11f, 1.0f);

    // CheckMark
    colors[ImGuiCol_CheckMark] = ubuntuOrange;

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ubuntuOrange;
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.28f, 0.11f, 1.0f);

    // Title
    colors[ImGuiCol_TitleBg] = darkGray;
    colors[ImGuiCol_TitleBgActive] = lighterGray;
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.16f, 0.93f);

    // For the window size
    ImGui::SetNextWindowSize(ImVec2(1200, 1200), ImGuiCond_Always);
}


void Menu::renderMenu() {

    if (!visible) 
    {
        ImGui::GetIO().MouseDrawCursor = false;
        ShowCursor(FALSE);
        return;
    }
    else 
    {
        // Force the OS cursor to be visible and remove clipping.
        ImGui::GetIO().MouseDrawCursor = true;
        ClipCursor(NULL);
        while (ShowCursor(TRUE) < 0);
        ReleaseCapture();
    }

    ImGui::Begin("ascend.gg", &visible);

    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Aimbot"))
        {
            
            ImGui::Checkbox("Enable Aimbot", &bAimbot);

            if (bAimbot)
            {
                ImGui::SliderFloat("Aimbot FOV", &aimbotFOV, 0.0f, 180.0f, "%.1f deg");
                ImGui::Combo("Target Mode", &targetSelectionIndex,
                    "Head\0Neck\0Chest\0");

                ImGui::Checkbox("Enable smoothing", &bEnableSmoothing);
                ImGui::SliderFloat("Aimbot smoothing", &aimbotSmoothing, 0.0f, 180.0f, "%.1f deg");
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ESP"))
        {
            ImGui::Checkbox("Enable ESP", &bEsp);
            if (bEsp)
            {
                ImGui::Checkbox("Show name", &bName);
                ImGui::Checkbox("Show health", &bHealth);
                ImGui::Checkbox("Show armor", &bArmor);
                ImGui::Checkbox("Show team", &bShowTeam);
                ImGui::Checkbox("Draw head", &bDrawHead);
                ImGui::ColorEdit4("Enemy esp color", boxEspColor);
                ImGui::ColorEdit4("Allied boxcolor", boxTeamEspColor);
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Misc"))
        {
            ImGui::Checkbox("Triggerbot", &bTriggerbot);
            if (bTriggerbot)
            {
                ImGui::SliderFloat("triggerbot delay", &triggerbotdelay, 0.0f, 250.0f, "%.1f deg");
            }
            ImGui::Checkbox("No recoil", &bNoRecoil);
            ImGui::Checkbox("Bunnyhop", &bBunnyHop);
            ImGui::Checkbox("Automatic", &bAutomatic);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Menu::render()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    renderMenu();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
