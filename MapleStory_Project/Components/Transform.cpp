#include "stdafx.h"
#include "Transform.h"
#include "Renders/ConstantBuffers/GlobalBuffers.h"


Transform::Transform(const std::string& name)
	: Component(name)
{
	worldBuffer = std::make_unique<WorldBuffer>();
}

void Transform::Update()
{
	if (bDirty == false)
		return;

	DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScalingFromVector(scale);
	DirectX::SimpleMath::Matrix R = DirectX::XMMatrixRotationZ(-rotation);
	DirectX::SimpleMath::Matrix T = DirectX::XMMatrixTranslationFromVector(position);

	world = S * R * T;
	worldBuffer->SetWorld(world);
	worldBuffer->Update();

	bDirty = false;
}

//=============================
// 위치 설정
// 기존 값과 동일하면 return
// 값 변경 시 Dirty 플래그 설정
//=============================
void Transform::SetPosition(DirectX::SimpleMath::Vector2 position)
{
	if (this->position == position) return;

	this->position = position;
	bDirty = true;
}

//=============================
// 스케일 설정
// 기존 값과 동일하면 return
// 값 변경 시 Dirty 플래그 설정
//=============================
void Transform::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	if (this->scale == scale) return;

	this->scale = scale;
	bDirty = true;
}

//=============================
// 회전 설정 (라디안)
// 기존 값과 동일하면 return
// 값 변경 시 Dirty 플래그 설정
//=============================
void Transform::SetRotationRadian(float radian)
{
	if (rotation == radian) return;

	rotation = radian;
	bDirty = true;
}

//=============================
// 회전 설정 (디그리, 도)
// 디그리(도)에서 라디안 변환 후 저장
// 기존 값과 동일하면 return
// 값 변경 시 Dirty 플래그 설정
//=============================
void Transform::SetRotationDegree(float degree)
{
	float radian = DirectX::XMConvertToRadians(degree);

	if (rotation == radian) return;

	rotation = radian;
	bDirty = true;
}

//===============================
// 위치 이동
// 매우 작은 이동은 무시
// 이동 발생 시 Dirty 플래그 설정
//===============================
void Transform::Move(DirectX::SimpleMath::Vector2 value)
{
	if (value.LengthSquared() < epsilon) return;

	position += value;
	bDirty = true;
}

//==========================
// 스케일 증가
// 매우 작은 변화는 무시
// 변경 시 Dirty 플래그 설정
//==========================
void Transform::AddScale(DirectX::SimpleMath::Vector2 value)
{
	if (value.LengthSquared() < epsilon) return;

	scale += value;
	bDirty = true;
}

//==========================
// 회전 증가 (라디안)
// 매우 작은 회전은 무시
// 변경 시 Dirty 플래그 설정
//==========================
void Transform::RotateRadian(float radian)
{
	if (abs(radian) < epsilon) return;

	rotation += radian;
	bDirty = true;
}

//======================================
// 회전 증가 (디그리, 도)
// 매우 작은 회전은 무시
// 디그리(도)에서 라디안으로 변환 후 누적
// 변경 시 Dirty 플래그 설정
//======================================
void Transform::RotateDegree(float degree)
{
	if (abs(degree) < epsilon) return;

	float radian = DirectX::XMConvertToRadians(degree);
	rotation += radian;
	bDirty = true;
}