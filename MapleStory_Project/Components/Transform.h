#pragma once
#include "Component.h"

//=======================================
// 2D 변환(Transform) 컴포넌트
// 위치, 스케일, 회전 관리
// 이동/회전/크기 변경 인터페이스 제공
// 변경 발생 시 Dirty로 행렬 재계산 트리거
//=======================================
class Transform : public Component
{
public:
	// 기본 Transform 생성(기본 이름: "Transform")
	Transform(const std::string& name = "Transform");

	// bDirty가 ture면 매 프레임 업데이트
	void Update() override;

	// 현재 위치 반환
	DirectX::SimpleMath::Vector2 GetPosition() const { return position; }
	// 현재 스케일 반환
	DirectX::SimpleMath::Vector2 GetScale() const { return scale; }
	// 현재 회전값(라디안) 반환
	float GetRotationRadian() const { return rotation; }
	// 현재 회전값(디그리, 도) 반환
	float GetRotationDegree() const { return DirectX::XMConvertToDegrees(rotation); }

	// 위치 설정
	void SetPosition(DirectX::SimpleMath::Vector2 position);
	// 스케일 설정
	void SetScale(DirectX::SimpleMath::Vector2 scale);
	// 회전 설정(라디안)
	void SetRotationRadian(float radian);
	// 회전 설정(디그리, 도)
	void SetRotationDegree(float degree);


	// 위치 이동
	void Move(DirectX::SimpleMath::Vector2 value);
	// 스케일 증가
	void AddScale(DirectX::SimpleMath::Vector2 value);
	// 회전 증가(라디안)
	void RotateRadian(float radian);
	// 회전 증가(디그리, 도)
	void RotateDegree(float degree);

private:
	DirectX::SimpleMath::Matrix world;
	std::unique_ptr<class WorldBuffer> worldBuffer;

	DirectX::SimpleMath::Vector2 position;	// 위치
	DirectX::SimpleMath::Vector2 scale;		// 스케일
	float rotation = 0.0f;					// 회전
	float epsilon = 1e-5f;					// 부동소수 비교 허용 오차(0.00001)
	bool bDirty = true;						// Transform 변경 여부 (행렬 재계산 필요)
};