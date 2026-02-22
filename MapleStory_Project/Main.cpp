#include "stdafx.h"

// 전역 Window 정보
HWND gHandle = nullptr; // 현재 생성된 Window의 Handle, 다른 시스템(Graphics 등)에서 Window 참조 시 사용
float gWinWidth = WIN_DEFAULT_WIDTH; // Window 기본 너비
float gWinHeight = WIN_DEFAULT_HEIGHT; // Window 기본 높이

// =================================
// 프로그램 시작 지점 (Entry Point)
// Win32 Application의 main 함수 역할
// =================================
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // 현재 프로그램 인스턴스 핸들
    _In_opt_ HINSTANCE hPrevInstance, // 이전 인스턴스 (사용 안함)
    _In_ LPWSTR    lpCmdLine, // 커맨드 라인 인자
    _In_ int       nCmdShow) // Window 표시 옵션
{
    // Window 생성에 필요한 정보 설정
    WinDesc w_desc;
    w_desc.appName = L"MapleStory_Project.HJB"; // Window 제목
    w_desc.instance = hInstance; // 프로그램 인스턴스
    w_desc.width = gWinWidth; // Window 너비
    w_desc.height = gWinHeight; // Window 높이

    auto window = std::make_unique<Window>(w_desc); // Window 객체 생성
    WPARAM wParam = window->Run(); // 메인 루프 실행

    return (int)wParam; // 프로그램 종료 코드 반환
}