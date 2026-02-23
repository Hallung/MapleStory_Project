#include "stdafx.h"
#include "Transform.h"
#include "Renders/ConstantBuffers/GlobalBuffers.h"


Transform::Transform(const std::string& name)
	: Component(name)
{
	// Transform의 월드 행렬을 GPU에 전달하기 위한 WorldBuffer 생성
	worldBuffer = std::make_unique<WorldBuffer>();
}

void Transform::Update()
{
	// Transform 값이 변경되지 않았다면 월드 행렬 갱신 불필요
	if (bDirty == false)
		return;

	// 스케일, 회전, 위치를 이용해 월드 행렬 구성(좌표계 보정으로 회전은 음수 적용)
	DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScalingFromVector(scale);
	DirectX::SimpleMath::Matrix R = DirectX::XMMatrixRotationZ(-rotation);
	DirectX::SimpleMath::Matrix T = DirectX::XMMatrixTranslationFromVector(position);

	world = S * R * T;

	// 계산된 월드 행렬을 GPU 상수버퍼에 반영
	worldBuffer->SetWorld(world);
	worldBuffer->Update();

	// 최신 상태로 동기화 되었으므로 Dirty 플래그 해제
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