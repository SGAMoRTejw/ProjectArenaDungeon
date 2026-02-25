#include "pch.h"
#include "Window.h"
#include "GameInstance.h"

std::unique_ptr<GameInstance> Window::gameInstance = nullptr;

Window::Window(const WinDesc& initDesc)
    :desc(initDesc)
{
    // 윈도우 클래스 등록
    WORD wHr = MyRegisterClass(desc);
    assert(wHr != 0);

    // 윈도우 생성
    gHandle = desc.handle = CreateWindowExW
    (
        WS_EX_APPWINDOW,
        desc.appName.c_str(),
        desc.appName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        HMENU(nullptr),
        desc.instance,
        nullptr
    );

    assert(desc.handle != nullptr);

    // 클라이언트 영역 크기를 기준으로 실제 윈도우 크기 설정
    RECT rect = { 0, 0, LONG(desc.width), LONG(desc.height) };

    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);

    const long& winWidth = rect.right - rect.left;
    const long& winHeight = rect.bottom - rect.top;

    // 화면 중앙 배치
    const UINT& winX = UINT((GetSystemMetrics(SM_CXSCREEN) - winWidth) * 0.5f);
    const UINT& winY = UINT((GetSystemMetrics(SM_CYSCREEN) - winHeight) * 0.5f);

    MoveWindow
    (
        desc.handle,
        winX,
        winY,
        winWidth,
        winHeight,
        true
    );

    ShowWindow(desc.handle, SW_SHOWNORMAL);
    UpdateWindow(desc.handle);

    // 커서 표시
    ShowCursor(true);
}

Window::~Window()
{
    // 윈도우 클래스 등록 해제
    UnregisterClassW(desc.appName.c_str(), desc.instance);
}

ATOM Window::MyRegisterClass(const WinDesc& initDesc)
{
    WNDCLASSEXW wcex = { 0 };

    // 윈도우 클래스 정보 설정
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = &WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = initDesc.instance;
    wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = HBRUSH(GetStockObject(GRAY_BRUSH));
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = initDesc.appName.c_str();
    wcex.hIconSm = wcex.hIcon;

    return RegisterClassExW(&wcex);
}

WPARAM Window::Run()
{
    gameInstance = std::make_unique<GameInstance>();
    gameInstance->Init();

    MSG msg;

    // 타겟 FPS 설정(144)
    TIME.SetTargetFPS(144.0);

    // 기본 메시지 루프입니다:
    while (true)
    {
        // 윈도우 메시지 처리
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Managers Update
            INPUT.Update();
            TIME.Update();

            gameInstance->Update();

            GRAPHICS.Begin();
            {
                gameInstance->Render();
            }
            GRAPHICS.End();

            // 프레임 제한
            TIME.WaitToTargetFrameRate();
        }
    }

    return msg.wParam;
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // 윈도우 종료 시 메시지 루프 종료 요청
        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}