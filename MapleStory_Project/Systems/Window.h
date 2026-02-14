#pragma once

// 창 설정 구조체
struct WinDesc
{
	std::wstring appName = L"";
	HINSTANCE instance = nullptr;
	HWND handle = nullptr;
	float width = 0;
	float height = 0;
};

class Window
{
public:
	Window(const WinDesc& initDesc);
	~Window();

	ATOM MyRegisterClass(const WinDesc& initDesc);
	WPARAM Run();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	WinDesc w_desc;

	static std::unique_ptr<class Program> program;
};