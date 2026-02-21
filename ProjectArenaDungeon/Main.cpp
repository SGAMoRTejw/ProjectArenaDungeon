#include "pch.h"
#include "Window.h"

HWND gHandle = nullptr;
float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WinDesc desc;
    desc.appName = L"ArenaDungeon";
    desc.instance = hInstance;
    desc.width = gWinWidth;
    desc.height = gWinHeight;

    auto window = std::make_unique<Window>(desc);
    WPARAM wParam = window->Run();

    return (int)wParam;
}