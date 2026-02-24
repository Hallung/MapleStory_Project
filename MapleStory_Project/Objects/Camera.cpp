#include "stdafx.h"
#include "Camera.h"
#include "Components/Transform.h"

Camera::Camera()
	: Object("MainCamera")
{
	viewProjectionBuffer = std::make_unique<ViewProjectionBuffer>();

	// Orthographic Projection 생성
	projection = DirectX::XMMatrixOrthographicOffCenterLH(0, gWinWidth, 0, gWinHeight, -1.0f, 1.0f);
	viewProjectionBuffer->SetProjection(projection);

	transform->SetScale({ 1.0f, 1.0f }); // Scale 값을 Zoom으로 사용, 초기 값은 1
}

void Camera::Update()
{
	__super::Update();

	// 현재 Transform 값 가져오기
	const DirectX::SimpleMath::Vector2 pos = transform->GetPosition();
	const float rot = transform->GetRotationRadian();
	const float zoom = transform->GetScale().x;

	// Transform 변경 없으면 갱신 생략
	if (pos == lastPos && rot == lastRot && zoom == lastZoom)
		return;

	// Cache 값 갱신
	lastPos = pos;
	lastRot = rot;
	lastZoom = zoom;

	// Camera View Matrix 생성 (Camera는 World의 반대로 이동해야 하므로 Inverse Transform)
	DirectX::SimpleMath::Matrix T = DirectX::XMMatrixTranslation(-pos.x, -pos.y, 0.0f);
	DirectX::SimpleMath::Matrix R = DirectX::XMMatrixRotationZ(-rot);
	DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScaling(zoom, zoom, 1.0f);

	view = T * R * S; // Inverse TRS 순서

	// GPU ConstantBuffer Update
	viewProjectionBuffer->SetView(view);
	viewProjectionBuffer->Update();
}

void Camera::Bind()
{
	viewProjectionBuffer->BindVS(1); // Shader Register Slot 1에 Bind

	__super::Render();
}

void Camera::SetPosition(DirectX::SimpleMath::Vector2 value)  // Camera 위치 설정
{
	transform->SetPosition(value);
}

void Camera::SetZoom(float value)
{
	// Zoom이 너무 작아지는 것 방지
	if (value < transform->GetEpsilon()) value = transform->GetEpsilon();
	transform->SetScale({ value, value });
}

float Camera::GetZoom()
{
	return transform->GetScale().x;
}

DirectX::SimpleMath::Vector2 Camera::ScreenToWorld(DirectX::SimpleMath::Vector2 screenPos)
{
	float zoom = GetZoom();
	DirectX::SimpleMath::Vector2 pos = transform->GetPosition();

	// Screen -> World 변환
	return (screenPos / zoom) + pos;
}

DirectX::SimpleMath::Vector2 Camera::WorldToScreen(DirectX::SimpleMath::Vector2 worldPos)
{
	float zoom = GetZoom();
	DirectX::SimpleMath::Vector2 pos = transform->GetPosition();

	// World -> Screen 변환
	return (worldPos - pos) * zoom;
}
