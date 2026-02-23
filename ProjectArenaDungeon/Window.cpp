#include "pch.h"
#include "Window.h"

Window::Window(const WinDesc& initDesc)
    :desc(initDesc)
{
    WORD wHr = MyRegisterClass(desc);
    assert(wHr != 0);

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

    RECT rect = { 0, 0, LONG(desc.width), LONG(desc.height) };

    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);

    const long& winWidth = rect.right - rect.left;
    const long& winHeight = rect.bottom - rect.top;

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

    ShowCursor(true);
}

Window::~Window()
{
    UnregisterClassW(desc.appName.c_str(), desc.instance);
}

ATOM Window::MyRegisterClass(const WinDesc& initDesc)
{
    WNDCLASSEXW wcex = { 0 };

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
    MSG msg;

    // FPS 144
    TIME.SetTargetFPS(144.0);

    // 기본 메시지 루프입니다:
    while (true)
    {
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

            GRAPHICS.Begin();
            {

            }
            GRAPHICS.End();

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
        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}