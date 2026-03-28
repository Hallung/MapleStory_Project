#pragma once
#include "Component.h"
#include "Collider.h"

// 몬스터의 능력치 및 전투 데이터를 관리하는 컴포넌트
class MonsterAbility : public Component
{
public:
	MonsterAbility() : Component("MonsterAbility") {}

	void Awake() override;

	void SetHelthPoint(UINT hp);	// 체력 설정
	UINT GetHelthPoint() const { return _hp; }	// 현재 체력 반환

	void SetMoveSpeed(float speed);	// 이동 속도 변경
	void ReturnSpeed();	// 이전 속도로 복귀
	UINT GetMoveSpeed() const { return _moveSpeed; }	// 이동 속도 반환

	void SetAttackPower(UINT power);	// 공격력 설정
	UINT GetAttackPower() const { return _attackPower; }	// 공격력 반환

	void TakeDamage(Collider* other, UINT damage);	// 데미지 적용

	void SetSoundPath(const std::string& path) { soundPath = path; }

private:
	UINT _hp = 100;				// 체력
	UINT _attackPower = 10;		// 공격력
	float _moveSpeed = 150.0f;	// 이동 속도

	float lastSpeed = 0.0f;	// 속도 변경 전 값 저장

	std::string soundPath = "";

private:
	std::shared_ptr<Collider> monsterCollider;
	std::shared_ptr<class MonsterState> monsterState;
	std::shared_ptr<class MonsterAggro> monsterAggro;
};