#include "stdafx.h"
#include "Window.h"

Window::Window(const WinDesc& initDesc)
    : w_desc(initDesc)
{
    WORD wHr = MyRegisterClass(w_desc); // Window Class 등록
    assert(wHr != 0);   // 클래스 등록 실패 여부 검사

    // 확장 창 스타일을 사용하여 겹치는, 팝업, 자식 창을 생성 가능
    // CreateWindow와 동일하게 사용 가능
    gHandle = w_desc.handle = CreateWindowExW
    (
        WS_EX_APPWINDOW,
        w_desc.appName.c_str(),
        w_desc.appName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        w_desc.width,
        w_desc.height,
        nullptr,
        HMENU(nullptr),
        w_desc.instance,
        nullptr
    );
    assert(w_desc.handle != nullptr); // 핸들이 있는지 검사

    RECT rect = { 0, 0, LONG(w_desc.width), LONG(w_desc.height) };

    // 클라이언트 사각형의 원하는 크기에 따라 창 사각형 크기 계산
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, 0);

    const long& winWidth = rect.right - rect.left;
    const long& winHeight = rect.bottom - rect.top;

    // 기본 디스플레이 모니터의 화면 너비를 통해 중간 위치 찾기
    const UINT& winX = (GetSystemMetrics(SM_CXSCREEN) - winWidth) * 0.5f;
    const UINT& winY = (GetSystemMetrics(SM_CYSCREEN) - winHeight) * 0.5f;

    // 창의 위치, 크기 변경
    MoveWindow
    (
        w_desc.handle,
        winX,
        winY,
        winWidth,
        winHeight,
        true        // 창을 다시 그린다
    );

    ShowWindow(w_desc.handle, SW_SHOWNORMAL);
    UpdateWindow(w_desc.handle);
}

Window::~Window()
{
    // 창 클래스의 등록을 취소하여 클래스에 필요한 메모리 해제
    UnregisterClassW(w_desc.appName.c_str(), w_desc.instance);
}

// 창 클래스 정보 등록
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

// 게임 루프
WPARAM Window::Run()
{
    MSG msg;

    while (true)
    {
        // 메시지가 없으면 대기하지 않고 즉시 반환
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            // 키 입력 메시지 변환
            TranslateMessage(&msg);

            // 메시지를 WndProc()으로 전달
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

// Window 이벤트 처리
LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // Window 종료 시 프로그램 종료 메시지 전달

        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}