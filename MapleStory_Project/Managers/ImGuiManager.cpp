#include "stdafx.h"
#include "ImGuiManager.h"
#include "Objects/Object.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Resources/Material.h"
#include "Components/Animator.h"

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

// 하나의 오브젝트에 대한 Inspector UI를 구성
// 오브젝트 이름을 최상위 노드로 표시하고, 그 아래에 각 컴포넌트 UI를 출력
void ImGuiManager::DrawObjectInspector(std::shared_ptr<Object> object, std::string name)
{
	// ImGui 위젯 ID 충돌 방지를 위해 오브젝트의 고유 주소를 ID로 사용
	ImGui::PushID(object.get());

	// 최상위 오브젝트 노드 스타일 설정
	ImGuiTreeNodeFlags flags =
		ImGuiTreeNodeFlags_DefaultOpen |   // DefaultOpen : 기본적으로 펼쳐진 상태
		ImGuiTreeNodeFlags_Framed |		   // Framed : 박스 형태로 강조
		ImGuiTreeNodeFlags_SpanAvailWidth; // SpanAvailWidth : 가용 영역 전체 폭 사용

	// 오브젝트 이름을 최상위 트리 노드로 생성
	if (ImGui::TreeNodeEx(name.c_str(), flags))
	{
		// 해당 오브젝트가 가지고 있는 컴포넌트 UI 출력
		DrawTransform(object);
		DrawMaterial(object);
		DrawAnimator(object);

		ImGui::TreePop();
	}

	// PushID에 대한 정리
	ImGui::PopID();
}

// Transform 컴포넌트 Inspector UI 출력
// Position, Scale, Rotation 값을 ImGui에서 직접 수정할 수 있다.
void ImGuiManager::DrawTransform(std::shared_ptr<Object> object)
{
	// Transform 컴포넌트 가져오기
	auto transform = object->GetTransform();

	// Transform이 없으면 출력하지 않음
	if (transform == nullptr) return;

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Position 편집
		DirectX::SimpleMath::Vector2 position = transform->GetPosition();
		if (ImGui::DragFloat2("Position", (float*)&position))
			transform->SetPosition(position);

		// Scale 편집
		DirectX::SimpleMath::Vector2 scale = transform->GetScale();
		if (ImGui::DragFloat2("Scale", (float*)&scale))
			transform->SetScale(scale);

		// Rotation 편집 (0 ~ 360도)
		float degree = transform->GetRotationDegree();
		if (ImGui::SliderFloat("Rotation", &degree, 0.0f, 360.0f))
			transform->SetRotationDegree(degree);
	}
}

// Material 컴포넌트 Inspector UI 출력
// Tint Color 값을 ImGui에서 실시간으로 변경 가능
void ImGuiManager::DrawMaterial(std::shared_ptr<Object> object)
{
	// MeshRenderer 컴포넌트 가져오기
	auto renderer = object->GetComponent<MeshRenderer>("MeshRenderer");

	// MeshRenderer 없으면 출력하지 않음
	if (renderer == nullptr) return;

	// Material 가져오기
	auto material = renderer->GetMaterial();

	// Material 없으면 출력하지 않음
	if (material == nullptr) return;

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DirectX::SimpleMath::Color objectColor = material->GetColor();

		// 색상 변경 시 Material에 즉시 반영
		if (ImGui::ColorEdit4("Tint Color", (float*)&objectColor, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaBar))
		{
			material->SetColor(objectColor);
		}
	}
}

// Animator 컴포넌트 Inspector UI 출력
// 애니메이션 재생, 클립 선택, 프레임 직접 제어 가능
void ImGuiManager::DrawAnimator(std::shared_ptr<Object> object)
{
	// Animator 컴포넌트 가져오기
	auto animator = object->GetComponent<Animator>("Animator");

	// Animator가 없으면 출력하지 않음
	if (animator == nullptr) return;

	if (ImGui::CollapsingHeader("Animator", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// wstring -> string 변환용 람다 (ImGui는 char 기반 문자열 사용)
		auto WStrToStr = [](const std::wstring& wstr) {
			std::string str(wstr.length(), '\0');
			for (size_t i = 0; i < wstr.length(); ++i)
				str[i] = static_cast<char>(wstr[i]);
			return str;
			};

		auto currentClip = animator->GetCurrentClip();

		// 재생 여부 토글
		bool bPlaying = animator->IsPlaying();
		if (ImGui::Checkbox("Play Animation", &bPlaying))
			animator->SetPlaying(bPlaying);

		std::string previewName = "None";

		// 현재 클립 이름 표시
		if (currentClip)
		{
			const std::wstring& wName = currentClip->GetName();
			previewName = WStrToStr(wName);
		}

		// 애니메이션 클립 선택 콤보박스
		if (ImGui::BeginCombo("Animation Clip", previewName.c_str()))
		{
			for (const auto& pair : animator->GetClips())
			{
				std::string clipName = WStrToStr(pair.first);
				bool  isSelected = (currentClip && currentClip->GetName() == pair.first);

				// 선택 시 해당 클립 재생
				if (ImGui::Selectable(clipName.c_str(), isSelected))
				{
					animator->Play(pair.first);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		// 현재 클립이 있을 경우 프레임 수동 조절 가능
		if (currentClip)
		{
			int maxFrame = (int)animator->GetCurrentClipFrameCount() - 1;
			int currentFrame = (int)animator->GetCurrentFrameIndex();

			// 프레임을 직접 조정하면 자동 재생은 중지
			if (ImGui::SliderInt("Frame Index", &currentFrame, 0, maxFrame))
			{
				animator->SetCurrentFrameIndex((UINT)currentFrame);
				animator->SetPlaying(false);
			}
		}
	}
}
