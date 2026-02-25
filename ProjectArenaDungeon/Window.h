#pragma once

class GameInstance;

// WinDesc
// - 윈도우 생성에 필요한 초기 설정 정보
// - 앱 이름, 인스턴스 핸들, 윈도우 핸들, 초기 크기 정보
struct WinDesc
{
	std::wstring appName = L"";
	HINSTANCE instance = nullptr;
	HWND handle = nullptr;
	float width = 0;
	float height = 0;
};

// Window
// - 윈도우 생성 및 메시지 루프를 관리하는 클래스
// - 윈도우 클래스 등록, 윈도우 생성, 메시지 루프(Run)
// - GameInstance를 생성하고 메인 루프에서 Update / Render 호출
class Window
{
public:
	// Window
	// - WinDesc를 기반으로 윈도우 생성
	Window(const WinDesc& initDesc);
	~Window();

	// MyRegisterClass
	// - 윈도우 클래스(WNDCLASSEX) 등록
	// - WndProc 콜백 함수 설정
	ATOM MyRegisterClass(const WinDesc& initDesc);

	// Run
	// - 메시지 루프 실행
	// - Managers(Update) 호출
	WPARAM Run();

private:
	// WndProc
	// - Win32 메시지 콜백 함수
	// - 시스템 메시지(WM_DESTROY 등) 처리
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	WinDesc desc;

	static std::unique_ptr<GameInstance> gameInstance;
};