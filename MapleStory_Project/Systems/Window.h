#pragma once

// Window 생성 시 필요한 초기 설정 정보 구조체
struct WinDesc
{
	std::wstring appName = L""; // 윈도우 클래스 이름 및 창 제목
	HINSTANCE instance = nullptr; // 프로그램 인스턴스 핸들
	HWND handle = nullptr; // 생성된 윈도우 핸들
	float width = 0; // 클라이언트 영역 너비
	float height = 0; // 클라이언트 영역 높이
};

// ======================================
// Win32 API 기반 Window 생성 및 관리 클래스
// 창 생성, 메시지 처리, 게임 루프 실행 담당
// ======================================
class Window
{
public:
	Window(const WinDesc& initDesc);
	~Window();

	ATOM MyRegisterClass(const WinDesc& initDesc); // Win32 Window Class 등록
	WPARAM Run(); // 메인 메시지 루프 실행

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // Win32 메시지 처리 함수

	WinDesc w_desc; // Window 생성 정보 저장

	static std::unique_ptr<class Game> game; // Window와 연결된 Game 실행 객체
};