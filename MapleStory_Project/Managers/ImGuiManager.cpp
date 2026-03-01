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

void ImGuiManager::ShowFPSOverlay()
{
	// 화면 가장자리와의 간격
	constexpr float DISTANCE = 10.0f;

	// 우측 상단 위치 계산 (pivot을 이용해 오른쪽 정렬)
	ImVec2 winPos = ImVec2(gWinWidth - DISTANCE, DISTANCE);
	ImVec2 winPosPivot = ImVec2(1.0f, 0.0f);

	ImGui::SetNextWindowPos(winPos, ImGuiCond_Always, winPosPivot);

	// 오버레이 창 옵션 설정
	ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoDecoration | // 타이틀바 및 테두리 제거
		ImGuiWindowFlags_AlwaysAutoResize |						// 내용에 맞게 자동 크기 조절
		ImGuiWindowFlags_NoSavedSettings |						// 위치/설정 저장하지 않음
		ImGuiWindowFlags_NoFocusOnAppearing |					// 생성 시 포커스 가져가지 않음
		ImGuiWindowFlags_NoNav |								// 키보드 네비게이션 비활성화
		ImGuiWindowFlags_NoMove |								// 창 이동 불가
		ImGuiWindowFlags_NoBackground;							// 배경 없이 텍스트만 표시

	ImGui::Begin("FPS Overlay", nullptr, winFlags);

	UINT fps = TimeManager::GetInstance().GetFPS();

	// FPS 및 Frame Time 문자열 생성
	char fpsText[64];
	sprintf_s(fpsText, "FPS : %d", fps);
	char msText[64];
	sprintf_s(msText, "Frame Time : %.3f ms", 1000.0f / fps);

	// 두 텍스트 중 더 긴 길이를 기준으로 정렬
	float width1 = ImGui::CalcTextSize(fpsText).x;
	float width2 = ImGui::CalcTextSize(msText).x;
	float maxWidth = max(width1, width2);

	// 오른쪽 정렬을 맞추기 위해 커서 위치 조정
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (maxWidth - width1));
	ImGui::TextUnformatted(fpsText);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (maxWidth - width2));
	ImGui::TextUnformatted(msText);

	ImGui::End();
}
