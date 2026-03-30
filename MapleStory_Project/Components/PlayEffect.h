#pragma once
#include "Component.h"

// 특정 애니메이션을 가진 이펙트를 재생하는 컴포넌트
class PlayEffect : public Component
{
public:
	PlayEffect() : Component("PlayEffect") {}

	void Awake() override;	// 초기화 시 Animator 컴포넌트 연결

	void Play();	// 이펙트 시작 (재생)
	
	void Stop();	// 이펙트 정지 (Stop 애니메이션 재생)

private:
	std::shared_ptr<class Animator> animator;
};