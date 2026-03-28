#pragma once
#include "Component.h"

class PlayEffect : public Component
{
public:
	PlayEffect() : Component("PlayEffect") {}

	void Awake() override;

	void Play();

	void Stop();

private:
	std::shared_ptr<class Animator> animator;
};