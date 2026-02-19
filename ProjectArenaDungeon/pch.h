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