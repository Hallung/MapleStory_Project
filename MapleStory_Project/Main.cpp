#include "stdafx.h"

HWND gHandle = nullptr;
float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WinDesc w_desc;
    w_desc.appName = L"MyProject";
    w_desc.instance = hInstance;
    w_desc.width = gWinWidth;
    w_desc.height = gWinHeight;

    auto window = std::make_unique<Window>(w_desc);
    WPARAM wParam = window->Run();

    return (int)wParam;
}