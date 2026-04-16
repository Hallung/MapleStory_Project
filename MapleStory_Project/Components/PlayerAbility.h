#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Player.h"

//============================================
// 플레이어의 능력치 정보를 관리하는 컴포넌트
// 캐릭터 성장, 데미지 계산, 이동 속도 등에 사용
//============================================
class PlayerAbility : public Component
{
public:
	PlayerAbility() : Component("PlayerAbility") {}
	
	void SetAbility(Player::Ability ability, float value);	// 특정 능력치 설정
	UINT GetAbility(Player::Ability ability) const;	// 특정 능력치 조회

	UINT GetAttackPower() const { return _attackPower; }	// 최종 공격력 반환

	// TODO: 추후 SetAttackPower 예시 -> _attackPower = _str * 2 + weaponDamage;

private:
	// 기본 스탯
	UINT _hp = 100;			// 체력
	UINT _mp = 100;			// 마나
	UINT _moveSpeed = 150;	// 이동 속도
	UINT _str = 4;			// 힘
	UINT _dex = 4;			// 민첩
	UINT _int = 4;			// 지능
	UINT _luk = 4;			// 행운

private:
	UINT _attackPower = 30;	// 최종 공격력 (데미지 계산에 사용)
};