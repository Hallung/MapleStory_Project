#include "stdafx.h"
#include "ImGuiManager.h"

// Win32 메시지를 ImGui로 전달하는 외부 핸들러
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 링크 오류 방지용 더미 선언
#if 0
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }
#endif

ImGuiManager::ImGuiManager() {}

void ImGuiManager::Init()
{
	IMGUI_CHECKVERSION(); // ImGui 버전 체크
	ImGui::CreateContext(); // ImGui 전역 컨텍스트 생성
	ImGuiIO& io = ImGui::GetIO(); (void)io; // IO 설정, 명시적인 void 캐스팅으로 경고 방지를 위한 관용구 사용
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 네비게이션 활성화

	ImGui::StyleColorsDark(); // 기본 다크 테마 적용

	// Win32 플랫폼/ DX11 렌더러 백엔드 초기화
	ImGui_ImplWin32_Init(gHandle);
	ImGui_ImplDX11_Init(Graphics::GetInstance().GetDevice().Get(), Graphics::GetInstance().GetDeviceContext().Get());
}

void ImGuiManager::Destroy()
{
	// 컨텍스트가 존재할 경우에만 종료 처리
	if (ImGui::GetCurrentContext())
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

void ImGuiManager::Update()
{
	// 백엔드 프레임 시작
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	
	// ImGui 프레임 시작
	ImGui::NewFrame();
}

void ImGuiManager::Render()
{
	// ImGui 내부 DrawData 생성
	ImGui::Render(); 

	// DX11으로 렌더링
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

LRESULT ImGuiManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ImGui 컨텍스트가 존재할 경우 메시지 전달
	if (ImGui::GetCurrentContext())
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	return 0;
}