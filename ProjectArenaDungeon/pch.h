#pragma once
#pragma once

#define NOMINMAX

#ifdef _DEBUG
#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

// Windows header
#include <windows.h>
#include <windowsx.h>

// C runtime header
#include <cassert>

// C++ runtime header
#include <string>
#include <memory>
#include <iostream>

// window runtime header
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// Extern Globals
extern HWND gHandle;
extern float gWinWidth;
extern float gWinHeight;

// constant expression
constexpr float epsilon = 1e-5f;

// window default size
#define WIN_DEFAULT_WIDTH 1280
#define WIN_DEFAULT_HEIGHT 720

// window center position
#define CENTER_X (gWinWidth * 0.5f)
#define CENTER_Y (gWinHeight * 0.5f)
#define CENTER (Vector2(CENTER_X, CENTER_Y))

// assert macro
#define CHECK(hr) do { assert(SUCCEEDED(hr)); } while(0)

// use singleton pattern
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