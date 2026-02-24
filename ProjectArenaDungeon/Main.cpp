#include "pch.h"
#include "Window.h"

// Globals
// - Window 생성에 사용되는 전역 핸들 및 크기 값
HWND gHandle = nullptr;
float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;

// Entry Point
// - 프로그램 진입점
// - Window 생성 및 Run()을 통해 메시지 루프를 실행한다.
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Window description
    // - 앱 이름, 인스턴스 핸들, 초기 윈도우 크기 설정
    WinDesc desc;
    desc.appName = L"ArenaDungeon";
    desc.instance = hInstance;
    desc.width = gWinWidth;
    desc.height = gWinHeight;

    // Create Window + run main Loop
    auto window = std::make_unique<Window>(desc);
    WPARAM wParam = window->Run();

    return (int)wParam;
}