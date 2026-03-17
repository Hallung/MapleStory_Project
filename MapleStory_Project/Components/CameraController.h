#pragma once
#include "Component.h"

// 씬 카메라 이동 및 줌 기능을 담당하는 컨트롤러
class CameraController : public Component
{
public:
	CameraController(float panSpeed = 1.0f, float zoomSpeed = 0.1f) : Component("CameraController"), panSpeed(panSpeed), zoomSpeed(zoomSpeed) {}

	void Update() override;

private:
	float panSpeed = 1.0f; // 카메라 평행 이동 속도
	float zoomSpeed = 0.1f; // 카메라 줌 속도

	DirectX::SimpleMath::Vector2 prevMousePos; // 이전 프레임 마우스 위치
};