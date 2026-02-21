#pragma once

// 디버그 상태에서 실행 시 콘솔창과 함께 실행
#ifdef _DEBUG
#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup")
#endif

// targetver(최고 수준의 가용성을 가진 Windows 플랫폼이 정의)
#include <SDKDDKVer.h>

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN

// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>

// C 런타임 헤더 파일
#include <cassert>

// C++ 런타임 헤더 파일
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <iostream>

// DirectX D3D11
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

// WRL
#include <wrl.h>

// DirectXTK
#include <DirectXTex.h>

// DirectXTK
#include "_Libraries/DirectXTK/SimpleMath.h"

extern HWND gHandle;
extern float gWinWidth;
extern float gWinHeight;

// 윈도우 사이즈 메크로
#define WIN_DEFAULT_WIDTH 1280
#define WIN_DEFAULT_HEIGHT 720

// assert 메크로
#define CHECK(hr) do { assert(SUCCEEDED(hr));} while(0)

// Systems
#include "Systems/Window.h"
#include "Systems/Graphics.h"

// Managers
#include "Managers/TimeManager.h"
#include "Managers/InputManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/PhysicsManager.h"