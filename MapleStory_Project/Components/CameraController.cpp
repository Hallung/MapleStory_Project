#include "stdafx.h"
#include "CameraController.h"
#include "Transform.h"
#include "Objects/Camera.h"
#include <algorithm>

void CameraController::Update()
{
    // 메인 카메라 및 Transform 참조
    auto camera = Camera::main;
    auto transform = camera->GetTransform();

    // 현재 마우스 위치
    DirectX::SimpleMath::Vector2 mouse = InputManager::GetInstance().GetMousePos();

    // 마우스 미들 버튼 드래그로 카메라 이동
    if (InputManager::GetInstance().GetKeyPress(VK_MBUTTON))
    {
        // 이전 프레임과의 마우스 이동량 계산
        DirectX::SimpleMath::Vector2 delta = mouse - prevMousePos;
        // 현재 카메라 위치
        DirectX::SimpleMath::Vector2 pos = transform->GetPosition();

        // 마우스 이동 방향에 따라 카메라 평행 이동
        pos.x -= delta.x * panSpeed;
        pos.y += delta.y * panSpeed;

        transform->SetPosition(pos);
    }

    // 다음 프레임을 위해 현재 마우스 위치 저장
    prevMousePos = mouse;
    // 마우스 휠 입력 값
    int wheel = InputManager::GetInstance().GetMouseWheel();

    if (wheel != 0)
    {
        // 현재 카메라 줌 값
        float zoom = camera->GetZoom();

        // 줌 변경 전 마우스 위치의 월드 좌표 계산 (마우스 기준 줌을 구현하기 위해 사용)
        DirectX::SimpleMath::Vector2 mouse = InputManager::GetInstance().GetMousePos();
        auto worldBefore = camera->ScreenToWorld(mouse);

        // 줌 값 변경
        zoom += wheel * zoomSpeed;
        // 줌 범위 제한
        zoom = std::clamp(zoom, 0.1f, 10.0f);

        camera->SetZoom(zoom);

        // 줌 변경 후 동일한 마우스 위치의 월드 좌표 계산
        auto worldAfter = camera->ScreenToWorld(mouse);

        // 줌으로 인해 이동한 월드 좌표 차이만큼 카메라 위치 보정 (마우스 위치를 기준으로 줌이 이루어지도록 유지)
        auto pos = transform->GetPosition();
        pos += (worldBefore - worldAfter);

        transform->SetPosition(pos);

        // 휠 입력 처리 후 값 초기화
        InputManager::GetInstance().ResetMouseWheel();
    }
}
