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

	void Awake() override;

	// 매 프레임 입력 기반 이동 및 애니메이션 업데이트
	void Update() override;

	// PlatformerController 업데이트 방지 상태 설정/반환
	void SetIsFinised(bool finished) { isFinished = finished; }
	bool GetIsFinished() const { return isFinished; }

private:
	// 이동 처리 (방향 벡터(dir)에 이동 속도를 곱해 위치 갱신
	void Move(DirectX::SimpleMath::Vector2 dir);

	// 점프 처리 (Y축 방향)에 jumpPower만큼 Impulse
	void Jump();

	// 피격 처리 함수(몬스터와 충돌 시 넉백 및 무적 시간 갱신 처리
	void Hit();

	// 공격 시스템을 처리하는 함수
	void Attack();

	// 포탈을 사용하는 함수
	void EnterPortal();

	// 상태를 업데이트 하는 함수
	void UpdateState();
	
	// 애니메이션 상태 업데이트(이동 방향에 따라 상태 변경)
	void UpdateAnimation(DirectX::SimpleMath::Vector2 dir);

	// 공격 가능 여부 반환
	bool GetCanAttack() const { return canAttack; }

	// target에 Damage를 주는 함수
	void ApplyDamage(Collider* target);

	// 공중 상태에서 플레이어의 수평 이동 감속 처리
	void ApplyAirControl(DirectX::SimpleMath::Vector2 dir);

	void ResetData();	// 데이터 초기화 함수

private:
	// 현재 무적 타이머
	float invincibleTimer = 3.0f;
	// 피격 후 다시 데미지를 받을 수 있기까지의 무적 지속 시간
	const float invincibleCooldown = 2.0f;

	bool isJump = false;	// 공중에 있는지 확인

	bool attackSignal = false;	// 공격 신호 여부
	bool canAttack = false;	// 공격 가능 여부
	bool playSound = false;	// 공격 소리 여부

	bool isFinished = false;	// 더이상 업테이트를 막기 위한 변수

private:
	// 데이터 캐싱
	std::shared_ptr<class Animator> animator;
	std::shared_ptr<Collider> playerCollider;
	std::shared_ptr<Collider> attackCollider;
	std::shared_ptr<class RigidBody> rigidBody;
	std::shared_ptr<class PlayerAbility> playerAbility;
	std::shared_ptr<class PlayerState> playerState;
	std::shared_ptr<class HitEvents> hitEvents;
};