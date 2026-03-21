#include "stdafx.h"
#include "PlatformerController.h"
#include "Objects/Object.h"
#include "Objects/DynamicObjects/Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Animator.h"
#include "HitEvents.h"
#include "PlayerState.h"
#include "PlayerAbility.h"
#include "Utilities/VirtualKey.h"
#include "Utilities/PhysicsUtils.h"

PlatformerController::PlatformerController()
	: Component("PlatformerController")
{
}
//===================================
// 매 프레임 입력 기반 이동 처리
// A/D 키 입력으로 좌우 방향 벡터 생성
// 계산된 방향으로 이동 수행
//===================================
void PlatformerController::Update()
{
	// 매 프레임 피격 판정 처리
	Hit();

	// 매 프레임 현재 플레이어 상태 확인
	UpdateState();

	DirectX::SimpleMath::Vector2 dir;

	if (InputManager::GetInstance().GetKeyPress(VK_LCONTROL))
	{
		if (!attackSignal)
		{
			attackSignal = true;
		}			
	}
	else if (!attackSignal)
	{
		// 오른쪽 이동 입력
		if (InputManager::GetInstance().GetKeyPress(VK_RIGHT))
		{
			dir.x += 1.0f;

			auto transform = GetOwner()->GetTransform();
			DirectX::SimpleMath::Vector2 scale = transform->GetScale();
			float absScaleX = fabsf(scale.x);
			if (scale.x > 0.0f)
				transform->SetScale({ -absScaleX, scale.y });
		}
		// 왼쪽 이동 입력
		if (InputManager::GetInstance().GetKeyPress(VK_LEFT))
		{
			dir.x -= 1.0f;

			auto transform = owner->GetTransform();
			DirectX::SimpleMath::Vector2 scale = transform->GetScale();
			float absScaleX = fabsf(scale.x);
			if (scale.x < 0.0f)
				transform->SetScale({ absScaleX, scale.y });
		}

		if (InputManager::GetInstance().GetKeyPress(VK_LMENU))
		{
			auto ownerState = GetOwner()->GetComponent<PlayerState>("PlayerState");

			// Player 상태 확인 (이미 점프 중이면 중복 점프 방지)
			if (ownerState->GetState() != Player::State::JUMPING && !isJump)
				Jump();
		}
	}
	Move(dir);

	Attack();

	UpdateAnimation(dir);
}


// 물리 기반 이동
void PlatformerController::Move(DirectX::SimpleMath::Vector2 dir)
{
	// 수평 입력이 없으면 이동 없음
	if (dir.x == 0.0f) return;

	// Object에 부착된 RigidBody 컴포넌트 가져오기
	auto rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");

	auto playerAbility = GetOwner()->GetComponent<PlayerAbility>("PlayerAbility");

	

	// 현재 Box2D 속도 조회 (중력 포함)
	b2Vec2 gravity = b2Body_GetLinearVelocity(rigidBody->GetBodyId());

	//물리 월드 좌표에서 화면 좌표계로 변환
	DirectX::SimpleMath::Vector2 gravityToScreen = PhysicsUtils::WorldToScreen(gravity);
	// RigidBody가 존재하고 Box2D Body가 유효한 경우에만 속도 설정
	// (물리 월드에 등록되지 않은 Body는 접근 방지)
	if (rigidBody && b2Body_IsValid(rigidBody->GetBodyId()))
	{
		// 입력 방향(dir)에 이동 속도(moveSpeed)를 곱해 속도 설정
		// 수직 속도는 기존 중력 값을 유지하여 자연스러운 낙하 유지
		rigidBody->SetVelocity(
			DirectX::SimpleMath::Vector2(dir.x * playerAbility->GetAbility(Player::Ability::SPEED) , gravityToScreen.y)
		);
	}
}

// TODO: 연속 점프 시 x축 방향 Velocity 유지되는 문제를 추후에 해결
void PlatformerController::Jump()
{
	const float jumpPower = 12.0f;	// 점프 시 가해질 임펄스 세기

	// RigidBody 컴포넌트 획득
	auto rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");

	// 현재 수직 속도 제거
	// (기존 낙하/상승 속도를 초기화하여 점프 높이 일정하게 유지)
	b2Vec2 vel = b2Body_GetLinearVelocity(rigidBody->GetBodyId());
	vel.y = 0.0f;
	b2Body_SetLinearVelocity(rigidBody->GetBodyId(), vel);

	// 위 방향으로 임펄스 적용
	b2Vec2 impulse(0.0f, jumpPower);
	b2Body_ApplyLinearImpulseToCenter(rigidBody->GetBodyId(), impulse, true);
}

// 몬스터와 충돌 시 피격 처리 수행
void PlatformerController::Hit()
{
	const float pushPower = 8.0f;

	// HitEvents 컴포넌트 가져오기
	auto hitEvent = GetOwner()->GetComponent<HitEvents>("HitEvents");

	auto ownerState = GetOwner()->GetComponent<PlayerState>("PlayerState");

	auto ownerCollider = GetOwner()->GetComponent<Collider>("PlayerCollider");

	// 무적 타이머 갱신
	if (invincibleTimer > 10.0f)
		invincibleTimer = 10.0f;	// 과도한 증가 방지
	else
		invincibleTimer += TimeManager::GetInstance().GetDeltaTime();

	// 몬스터와 충돌 중인지 확인
	if (hitEvent->IsColliding(ownerCollider.get(), CollisionLayer::Monster))
	{
		// 무적 시간이 끝났을 때만 피격 처리
		if (invincibleTimer >= invincibleCooldown)
		{
			auto rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");

			// 기존 수평 속도 제거 (넉백 방향 명확화)
			b2Vec2 velocity = b2Body_GetLinearVelocity(rigidBody->GetBodyId());
			velocity.x = 0.0f;
			b2Body_SetLinearVelocity(rigidBody->GetBodyId(), velocity);

			// 몬스터 위치 기반 넉백 방향 계산
			b2Vec2 monsterPos = hitEvent->GetMonsterPosition();
			b2Vec2 ownerPos = b2Body_GetPosition(rigidBody->GetBodyId());

			// 플레이어 - 몬스터 방향 벡터
			float dirVec = ownerPos.x - monsterPos.x;

			// 좌/우 넉백 방향 결정
			float dir = (dirVec > 0) ? 1.0f : -1.0f;


			// 위 + 좌우 방향 임펄스 적용
			if (ownerState->GetState() == Player::State::JUMPING)
			{
				b2Vec2 impulse(dir * pushPower, 0.0f);
				b2Body_ApplyLinearImpulse(rigidBody->GetBodyId(), impulse, monsterPos, true);
			}
			else
			{
				b2Vec2 impulse(dir * pushPower, pushPower - 5.0f);
				b2Body_ApplyLinearImpulse(rigidBody->GetBodyId(), impulse, monsterPos, true);
			}
			
			// 무적 시간 (타이머 초기화)
			invincibleTimer = 0.0f;
		}
	}
}

void PlatformerController::Attack()
{
	auto animator = GetOwner()->GetComponent<Animator>("Animator");
	auto hitEvent = GetOwner()->GetComponent<HitEvents>("HitEvents");

	UINT clipCurrentIndex = animator->GetCurrentFrameIndex();
	std::wstring clipName = animator->GetCurrentClip()->GetName();

	if (clipName == L"Attack" && clipCurrentIndex == 2)
	{
		currClipRate += TimeManager::GetInstance().GetDeltaTime();

		auto attackCol = GetOwner()->GetComponent<Collider>("AttackCollider");

		if (hitEvent->IsColliding(attackCol.get(), CollisionLayer::Monster) &&
			canAttack)
		{
			canAttack = false;
		}

		if (currClipRate >= 0.25f)
		{
			attackSignal = false;
			currClipRate = 0.0f;
		}
	}
	else
	{
		canAttack = true;
	}
}

// 현재 플레이어 상황을 기반으로 최종 State를 하나의 규칙으로 결정
void PlatformerController::UpdateState()
{
	// Owner 객체에 붙어있는 Collider 컴포넌트를 가져옴
	auto collider = GetOwner()->GetComponent<Collider>("PlayerCollider");

	auto ownerState = GetOwner()->GetComponent<PlayerState>("PlayerState");
	
	// 지면 충돌 확인
	bool grounded = collider->CheckGrounded();
	
	// 현재 상태 저장
	Player::State newState;

	isJump = grounded ? false : true;

	if (attackSignal)
	{
		newState = Player::State::ATTACKING;
	}
	else
	{
		// Collider의 CheckGrounded()를 통해 현재 플레이어가 지면에 닿아 있는지 여부를 확인
		if (isJump)
		{
			// 공중 상태일 때

			// 현재 상태가 이미 Jumping이 아니라면 상태를 Jumping으로 변경
			newState = Player::State::JUMPING;
		}
		else
		{
			// 지면에 닿아 있는 상태

			if (invincibleTimer > invincibleCooldown)
			{
				// 무적 시간이 끝났다면 기본 대기 상태
				newState = Player::State::STANDING;
			}
			else
			{
				// 무적 시간 진행 중 -> 피격 상태 유지
				newState = Player::State::HITTING;
			}
		}
	}

	// 현재 상태가 newState와 다르면 newState 상태로 변경
	if (ownerState->GetState() != newState)
	{
		ownerState->SetState(newState);
	}
}


void PlatformerController::UpdateAnimation(DirectX::SimpleMath::Vector2 dir)
{
	// Object에 부착된 Animator 컴포넌트 가져오기
	auto animator = GetOwner()->GetComponent<Animator>("Animator");

	auto ownerState = GetOwner()->GetComponent<PlayerState>("PlayerState");

	// Animator가 존재하지 않으면 애니메이션 업데이트 불가
	if (animator == nullptr) return;

	if (ownerState->GetState() == Player::State::ATTACKING)
	{
		animator->Play(L"Attack");
	}
	else
	{
		// Player 상태가 Jumping일 경우 Jump 애니메이션 재생
		if (ownerState->GetState() == Player::State::JUMPING)
		{
			animator->Play(L"Jump");
		}
		// Player 상태가 Jumping이 아니면 다른 애니메이션 재생
		else
		{
			// 좌우 이동 입력이 존재하면 Move 애니메이션 재생
			if (dir.x != 0.0f)
				animator->Play(L"Move");
			else
			{
				// 이동하지 않는 경우 상태 기반 애니메이션 선택
				if (ownerState->GetState() == Player::State::HITTING)
				{
					// 피격 중
					animator->Play(L"Hit");
				}
				else
				{
					// 기본 대기 상태
					animator->Play(L"Stand");
				}
			}
		}
	}
}
