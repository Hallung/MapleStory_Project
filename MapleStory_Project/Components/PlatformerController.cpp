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
#include "MonsterAbility.h"
#include "Utilities/VirtualKey.h"
#include "Utilities/PhysicsUtils.h"

PlatformerController::PlatformerController()
	: Component("PlatformerController")
{
}

void PlatformerController::Awake()
{
	// Player가 가지고 있는 컴포넌트 정보를 캐싱
	animator = GetOwner()->GetComponent<Animator>("Animator");
	playerCollider = GetOwner()->GetComponent<Collider>("PlayerCollider");
	attackCollider = GetOwner()->GetComponent<Collider>("AttackCollider");
	rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");
	playerAbility = GetOwner()->GetComponent<PlayerAbility>("PlayerAbility");
	playerState = GetOwner()->GetComponent<PlayerState>("PlayerState");
	hitEvents = GetOwner()->GetComponent<HitEvents>("HitEvents");
}

//===================================
// 매 프레임 입력 기반 이동 처리
// A/D 키 입력으로 좌우 방향 벡터 생성
// 계산된 방향으로 이동 수행
//===================================
void PlatformerController::Update()
{
	if (isFinished) return;

	// 매 프레임 피격 판정 처리
	Hit();

	// 매 프레임 현재 플레이어 상태 확인
	UpdateState();

	DirectX::SimpleMath::Vector2 dir;

	if (InputManager::GetInstance().GetKeyPress(VK_UP))
	{
		EnterPortal();
	}

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
			// 현재 방향 설정
			dir.x += 1.0f;

			DirectX::SimpleMath::Vector2 offset = attackCollider->GetOffsetData();
			auto transform = GetOwner()->GetTransform();
			DirectX::SimpleMath::Vector2 scale = transform->GetScale();
			float absScaleX = fabsf(scale.x);
			if (scale.x > 0.0f)
			{
				// 스케일을 바꿔 캐릭터 뒤집기
				transform->SetScale({ -absScaleX, scale.y });
			}
			if (offset.x < 0.0f)
			{
				// 캐릭터의 앞에 AttackCollider가 올 수 있도록 Offset 재설정
				attackCollider->SetOffset(DirectX::SimpleMath::Vector2(-offset.x, offset.y));
			}
		}
		// 왼쪽 이동 입력
		if (InputManager::GetInstance().GetKeyPress(VK_LEFT))
		{
			// 현재 방향 설정
			dir.x -= 1.0f;

			DirectX::SimpleMath::Vector2 offset = attackCollider->GetOffsetData();
			auto transform = owner->GetTransform();
			DirectX::SimpleMath::Vector2 scale = transform->GetScale();
			float absScaleX = fabsf(scale.x);
			if (scale.x < 0.0f)
			{
				// 스케일을 바꿔 캐릭터 뒤집기
				transform->SetScale({ absScaleX, scale.y });	
			}
			if (offset.x > 0.0f)
			{
				// 캐릭터의 앞에 AttackCollider가 올 수 있도록 Offset 재설정
				attackCollider->SetOffset(DirectX::SimpleMath::Vector2(-offset.x, offset.y));
			}
		}
		
		if (InputManager::GetInstance().GetKeyPress(VK_LMENU))
		{			
			// Player 상태 확인 (이미 점프 중이면 중복 점프 방지)
			if (playerState->GetState() != Player::State::JUMPING && !isJump)
				Jump();
		}
	}
	// 매 프레임 dir 값에 따라 Move 설정
	Move(dir);

	// 매 프레임 Attack 신호에 따라 Attack 설정
	Attack();

	// 매 프레임 상태에 따라 애니메이션 설정
	UpdateAnimation(dir);

	// 매 프레임 점프 상태와 dir에 따라 공중 감속 설정
	ApplyAirControl(dir);
}


// 물리 기반 이동
void PlatformerController::Move(DirectX::SimpleMath::Vector2 dir)
{
	// 수평 입력이 없으면 이동 없음
	if (dir.x == 0.0f) return;

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

// 물리 기반 점프
void PlatformerController::Jump()
{
	const float jumpPower = 12.0f;	// 점프 시 가해질 임펄스 세기

	//========================================================
	// 현재 수직 속도 제거
	// (기존 낙하/상승 속도를 초기화하여 점프 높이 일정하게 유지)
	//========================================================
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

	// 무적 타이머 갱신
	if (invincibleTimer > 10.0f)
		invincibleTimer = 10.0f;	// 과도한 증가 방지
	else
		invincibleTimer += TimeManager::GetInstance().GetDeltaTime();

	// 몬스터와 충돌 중인지 확인
	if (hitEvents->IsColliding(playerCollider.get(), CollisionLayer::Monster))
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
			b2Vec2 monsterPos = hitEvents->GetMonsterPosition();
			b2Vec2 ownerPos = b2Body_GetPosition(rigidBody->GetBodyId());

			// 플레이어 - 몬스터 방향 벡터
			float dirVec = ownerPos.x - monsterPos.x;

			// 좌/우 넉백 방향 결정
			float dir = (dirVec > 0) ? 1.0f : -1.0f;


			// 위 + 좌우 방향 임펄스 적용
			if (playerState->GetState() == Player::State::JUMPING)
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
	// AttackCollider과 가장 가까운 충돌체의 정보를 저장
	auto nearsetTarget = hitEvents->GetNearestTarget(attackCollider.get(), CollisionLayer::Monster);

	// 애니메이션의 현재 인덱스 정보를 저장
	UINT clipCurrentIndex = animator->GetCurrentFrameIndex();
	// 애니메이션 이름 정보를 저장
	std::wstring clipName = animator->GetCurrentClip()->GetName();

	// 애니메이션 이름이 Attack이고 현재 인덱스가 2 일때 실행
	if (clipName == L"Attack" && clipCurrentIndex == 2)
	{
		// 가까운 타겟이 있고 공격 가능 상태이면 실행
		if (nearsetTarget && canAttack)
		{
			// 타겟에 데미지 주기
			ApplyDamage(nearsetTarget);
			// 타겟을 공격 했으면 해당 애니메이션이 끝날때 까지 공격 불가
			canAttack = false;
		}

		if (animator->IsFinished())
		{
			// 애니메이션이 끝나면 다시 공격 가능한 신호 설정
			attackSignal = false;
		}
	}
	else
	{
		// 위 조건이 아닐 경우 공격 가능 상태 유지
		canAttack = true;
	}
}

void PlatformerController::EnterPortal()
{
	if (hitEvents->IsColliding(playerCollider.get(), CollisionLayer::Portal))
	{
		
	}
}

// 현재 플레이어 상황을 기반으로 최종 State를 하나의 규칙으로 결정
void PlatformerController::UpdateState()
{
	// 지면 충돌 확인
	bool grounded = playerCollider->CheckGrounded();
	
	// 현재 상태 저장
	Player::State newState;

	// 지면에 충돌하면 isJump를 flase 아닐결우는 true로 저장
	isJump = grounded ? false : true;

	// 공격 신호가 들어오면 최우선으로 상태 설정
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
	if (playerState->GetState() != newState)
	{
		playerState->SetState(newState);
	}
}

// 현재 State에 따라 애니메이션 설정
void PlatformerController::UpdateAnimation(DirectX::SimpleMath::Vector2 dir)
{
	// Animator가 존재하지 않으면 애니메이션 업데이트 불가
	if (animator == nullptr) return;

	//Player 상태가 Attack일 경우 Attack 애니메이션 재생
	if (playerState->GetState() == Player::State::ATTACKING)
	{
		animator->Play(L"Attack");
	}
	else
	{
		// Player 상태가 Jumping일 경우 Jump 애니메이션 재생
		if (playerState->GetState() == Player::State::JUMPING)
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
				if (playerState->GetState() == Player::State::HITTING)
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

//============================================================
// 플레이어의 공격이 Monster Collider에 적중했을 때 데미지 적용
// 실제 데미지 계산 및 HP 처리는 Ability가 담당
//============================================================
void PlatformerController::ApplyDamage(Collider* target)
{
	auto ability = target->GetOwner()->GetComponent<MonsterAbility>("MonsterAbility");
	
	// 플레이어 공격력 가져오기
	UINT damage = playerAbility->GetAttackPower();

	// MonsterAbility가 존재할 경우 데미지 적용
	if (ability)
		ability->TakeDamage(attackCollider.get(), damage);
}

//=================================================
// 공중 상태에서 플레이어의 수평 이동 감속 처리
// Box2D에서는 공중 상태일 때 마찰이 적용되지 않아
// 이동 키를 떼어도 기존 X 속도가 계속 유지
// 입력이 없을 경우 X 속도를 0 방향으로 서서히 보간
//=================================================
void PlatformerController::ApplyAirControl(DirectX::SimpleMath::Vector2 dir)
{
	if (!isJump) return;	// 점프 상태가 아니면 공중 제어 불필요

	// 현재 물리 속도 조회
	b2Vec2 vel = b2Body_GetLinearVelocity(rigidBody->GetBodyId());

	// 수평 입력이 없는 경우 -> 공중 감속 적용
	if (abs(dir.x) < 0.01f)
	{
		const float airDrag = 1.0f;	// 공중 감속 계수 (튜닝 값)
		float deltaTime = TimeManager::GetInstance().GetDeltaTime();

		// 목표 속도(0)로 점진적 감속
		vel.x = vel.x - (vel.x * airDrag * deltaTime);
	}

	// 수정된 속도를 Box2D Body에 적용
	b2Body_SetLinearVelocity(rigidBody->GetBodyId(), vel);
}
