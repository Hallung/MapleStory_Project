#pragma once
#include "Object.h"
#include "Renders/ConstantBuffers/GlobalBuffers.h"

//===========================================
// Scene을 화면에 렌더링하기 위한 Camera Object
// Transform 기반으로 위치, 회전, Zoom 관리
//===========================================
class Camera : public Object
{
public:
	Camera();

	void Update() override; // Transform 변경 시 View Matrix 갱신

	void Bind(); // ViewProjectionBuffer를 Shader에 Bind

	void SetPosition(DirectX::SimpleMath::Vector2 value); // Camera 위치 설정
	void SetZoom(float value); // Camera Zoom 설정
	float GetZoom(); // 현재 Zoom 반환

	DirectX::SimpleMath::Vector2 ScreenToWorld(DirectX::SimpleMath::Vector2 screenPos); // Screen -> World 좌표 변환
	DirectX::SimpleMath::Vector2 WorldToScreen(DirectX::SimpleMath::Vector2 worldPos); // World -> Screen 좌표 변환

	DirectX::SimpleMath::Matrix GetView() const { return view; } // 현재 View Matrix 반환
	DirectX::SimpleMath::Matrix GetProjection() const { return projection; } // 현재 Projection Matrix 반환

private:
	DirectX::SimpleMath::Matrix view; // Camera View Matrix (Transform 기반 계산)
	DirectX::SimpleMath::Matrix projection; // Camera Projection Matrix (Orthographic)
	std::unique_ptr<class ViewProjectionBuffer> viewProjectionBuffer; // Shader에 전달할 ViewProjection ConstantBuffer

	// Transform 변경 여부 확인용 Cache
	DirectX::SimpleMath::Vector2 lastPos = { -9999.0f, -9999.0f };
	float lastRot = 9999.9f;
	float lastZoom = -9999.0f;
};