#pragma once

//==============================================
// 싱글턴 기반 ImGui 관리 클래스
// ImGui의 초기화, 프레임 갱신, 렌더링, 종료를 담당
// Win32 + DX11 백엔드 기반으로 동작
//==============================================
class ImGuiManager
{
private:
	ImGuiManager();
	~ImGuiManager() = default;

public:
	// 싱글턴 복사/이동 금지
	ImGuiManager(const ImGuiManager& other) = delete;
	ImGuiManager& operator=(const ImGuiManager& other) = delete;
	ImGuiManager(ImGuiManager&& other) = delete;
	ImGuiManager& operator=(ImGuiManager&& other) = delete;

	// 싱글턴 인스턴스 반환
	static ImGuiManager& GetInstance()
	{
		static ImGuiManager instance;
		return instance;
	}

public:
	void Init(); // ImGui 컨텍스트 생성 및 플랫폼/렌더러 초기화
	void Destroy(); // ImGui 백엔드 종료 및 컨텍스트 해제
	void Update(); // 새로운 ImGui 프레임 시작
	void Render(); // ImGui DrawData를 DX11으로 렌더링

	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Win32 메시지를 ImGui에 전달
};