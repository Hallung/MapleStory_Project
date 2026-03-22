#pragma once
#include "Component.h"



//=======================================
// 2D 플렛폼 캐릭터 이동 컨트롤러 컴포넌트
// 좌우 이동입력 처리 및 속도 적용
// 이동 방향에 따른 애니메이션 상태 갱신
//=======================================
class PlatformerController : public Component
{
public:
	//===============================================
	// 기본 이동 속도를 가지는 컨트롤러 생성
	// Component 이름을 "PlatformerController"로 설정
	//===============================================
	PlatformerController();

	// 매 프레임 입력 기반 이동 및 애니메이션 업데이트
	void Update() override;

private:
	// 이동 처리 (방향 벡터(dir)에 이동 속도를 곱해 워치 갱신
	void Move(DirectX::SimpleMath::Vector2 dir);

	// 점프 처리 (Y축 방향)에 jumpPower만큼 Impulse
	void Jump();

	// 피격 처리 함수(몬스터와 충돌 시 넉백 및 무적 시간 갱신 처리
	void Hit();

	void Attack();

	void UpdateState();
	
	// 애니메이션 상태 업데이트(이동 방향에 따라 상태 변경)
	void UpdateAnimation(DirectX::SimpleMath::Vector2 dir);

	bool GetCanAttack() const { return canAttack; }

	void ApplyDamage(Collider* target);

	void ApplyAirControl(DirectX::SimpleMath::Vector2 dir);

private:
	// 현재 무적 타이머
	float invincibleTimer = 3.0f;
	// 피격 후 다시 데미지를 받을 수 있기까지의 무적 지속 시간
	const float invincibleCooldown = 2.0f;

	bool isJump = false;

	bool attackSignal = false;
	bool canAttack = false;
};