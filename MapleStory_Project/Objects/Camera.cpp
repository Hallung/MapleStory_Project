#include "stdafx.h"
#include "Camera.h"
#include "Components/Transform.h"
#include "Objects/TileMap.h"
#include <algorithm>

// 초기 상태에서는 메인 카메라가 없으므로 nullptr로 설정
Camera* Camera::main = nullptr;

Camera::Camera()
	: Object("MainCamera")
{
	viewProjectionBuffer = std::make_unique<ViewProjectionBuffer>();

	// Orthographic Projection 생성
	projection = DirectX::XMMatrixOrthographicOffCenterLH(0, gWinWidth, 0, gWinHeight, -1.0f, 1.0f);
	viewProjectionBuffer->SetProjection(projection);

	transform->SetScale(defaultScale); // Scale 값을 Zoom으로 사용, 초기 값은 1
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

// 카메라 상태를 초기값으로 되돌리는 함수
void Camera::Reset()
{
	transform->SetPosition(defaultPosition); // 카메라 위치를 기본 위치로 초기화
	transform->SetScale(defaultScale); // 카메라 줌을 기본값으로 초기화
}

// 타겟 위치를 기준으로 카메라를 이동시키되, 타일맵 범위를 벗어나지 않도록 Clamp 처리하는 함수
void Camera::SetPositionWithClamp(DirectX::SimpleMath::Vector2 targetPos, TileMap* tileMap)
{
	// 타일맵이 없는 경우 Clamp 없이 그대로 위치 설정
	if (!tileMap)
	{
		transform->SetPosition(targetPos);
		return;
	}

	// 최종적으로 적용할 카메라 위치
	DirectX::SimpleMath::Vector2 pos = targetPos;

	// target을 화면 중앙에 위치시키기 위한 보정
	pos.x -= gWinWidth * 0.5f;
	pos.y -= gWinHeight * 0.5f;

	// 타일맵 전체 월드 크기 계산
	float mapWidth = tileMap->GetWidth() * tileMap->GetTileSize();
	float mapHeight = tileMap->GetHeight() * tileMap->GetTileSize();

	// Clamp 최소값 (좌하단 기준)
	float minX = 0.0f;
	float minY = 0.0f;

	// Clamp 최대값 (맵 끝 - 화면 크기)
	// 화면이 맵 밖으로 나가지 않도록 제한
	float maxX = mapWidth - gWinWidth;
	float maxY = mapHeight - gWinHeight;

	// X축 Clamp 처리
	// 맵이 화면보다 작은 경우 -> 카메라를 맵 중앙에 고정
	if (mapWidth <= gWinWidth)
		pos.x = (mapWidth - gWinWidth) * 0.5f;
	else
		pos.x = std::clamp(pos.x, minX, maxX);

	// Y축 Clamp 처리
	if (mapHeight <= gWinHeight)
		pos.y = (mapHeight - gWinHeight) * 0.5f;
	else
		pos.y = std::clamp(pos.y, minY, maxY);

	// 최종 계산된 위치 적용
	transform->SetPosition(pos);
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
