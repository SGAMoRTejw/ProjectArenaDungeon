#pragma once

// pch.h
// - Precompiled Header: 컴파일 시간을 줄이기 위한 공용 포함 헤더
// - 시스템/표준 라이브러리, 공용 타입/매크로를 포함 한다.
// - 프로젝트 로직과 관련된 헤더는 최소화 한다.

#define NOMINMAX				// Window.h의 min/max 매크로 충돌 방지

#ifdef _DEBUG					// 디버그 빌드에서 콘솔 출력 확인용
#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN		// Window 헤더의 불필요 항목을 제외를 통한 최적화

// Windows header
#include <windows.h>
#include <windowsx.h>

// C runtime header
#include <cassert>

// C++ runtime header
#include <string>
#include <memory>
#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <span>
#include <unordered_map>

// DirectX D3D11
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// WRL (ComPtr)
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// DirectXTK SimpleMath
#include "_Libraries/DirectXTK/SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

// Extern Globals
// - Window 생성 및 런타임에서 초기화 되는 전역 값
extern HWND gHandle;
extern float gWinWidth;
extern float gWinHeight;

// Constants
constexpr float epsilon = 1e-5f;

// Window default size
#define WIN_DEFAULT_WIDTH 1280
#define WIN_DEFAULT_HEIGHT 720

// Window helper macros
// - gWinWidth와 gWinHeight는 변경될 수 있으므로 주의
#define CENTER_X (gWinWidth * 0.5f)
#define CENTER_Y (gWinHeight * 0.5f)
#define CENTER (Vector2(CENTER_X, CENTER_Y))

// CHECK
// - HRESULT 실패를 디버그 단계에서 검출하기 위한 assert 매크로
#define CHECK(hr) do { assert(SUCCEEDED(hr)); } while(0)

// DECLARE_SINGLETON
// - Managers\에서 사용하는 싱글톤 패턴 매크로
#define DECLARE_SINGLETON(CLASS_NAME)							\
private:														\
CLASS_NAME();													\
~CLASS_NAME() = default;										\
public:															\
CLASS_NAME(const CLASS_NAME& other) = delete;					\
CLASS_NAME& operator=(const CLASS_NAME& other) = delete;		\
CLASS_NAME(CLASS_NAME&& other) = delete;						\
CLASS_NAME& operator=(CLASS_NAME&& other) = delete;				\
static CLASS_NAME& GetInstance()								\
{																\
	static CLASS_NAME instance;									\
	return instance;											\
}

// Managers
#include "Managers/TimeManager.h"
#include "Managers/InputManager.h"
#include "Managers/GraphicsManager.h"

// Manager access macros
// - 반복되는 접근 코드를 간결하게 사용하기 위한 편의성 매크로
#define TIME TimeManager::GetInstance()
#define DELTA TIME.GetDeltaTime()
#define INPUT InputManager::GetInstance()
#define GRAPHICS GraphicsManager::GetInstance()
#define DEVICE GRAPHICS.GetDevice()
#define DEVICE_CONTEXT GRAPHICS.GetDeviceContext()