#include "stdafx.h"
#include "PlatformerController.h"
#include "Objects/Object.h"
#include "Objects/DynamicObjects/Player.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Animator.h"
#include "HitEvents.h"
#include "Utilities/VirtualKey.h"
#include "Utilities/PhysicsUtils.h"

PlatformerController::PlatformerController(float moveSpeed)
	: Component("PlatformerController"), moveSpeed(moveSpeed)
{
	// PlatformerController 생성 시 Player 관리 클래스 객체 생성
	player = std::make_shared<Player>();
}

//===================================
// 매 프레임 입력 기반 이동 처리
// A/D 키 입력으로 좌우 방향 벡터 생성
// 계산된 방향으로 이동 수행
//===================================
void PlatformerController::Update()
{
	// Owner 객체에 붙어있는 Collider 컴포넌트를 가져옴
	auto collider = GetOwner()->GetComponent<Collider>("Collider");

	// Collider의 CheckGrounded()를 통해 현재 플레이어가 지면에 닿아 있는지 여부를 확인
	if (!collider->CheckGrounded())
	{
		// 공중 상태일 때

		// 현재 상태가 이미 Jumping이 아니라면 상태를 Jumping으로 변경
		if (player->GetState() != Player::State::JUMPING)
			player->SetState(Player::State::JUMPING);
	}
	else
	{
		// 지면에 닿아 있는 상태

		// 현재 상태가 Standing이 아니라면 상태를 Standing으로 변경
		if (player->GetState() != Player::State::STANDING)
			player->SetState(Player::State::STANDING);
	}

	DirectX::SimpleMath::Vector2 dir;
	// 오른쪽 이동 입력
	if (InputManager::GetInstance().GetKeyPress(VK_D)) 
	{
		dir.x += 1.0f;

		auto transform = GetOwner()->GetTransform();
		DirectX::SimpleMath::Vector2 scale = transform->GetScale();
		float absScaleX = fabsf(scale.x);
		if (scale.x > 0.0f)
			transform->SetScale({ -absScaleX, scale.y });
	}
	// 왼쪽 이동 입력
	if (InputManager::GetInstance().GetKeyPress(VK_A)) 
	{
		dir.x -= 1.0f;

		auto transform = owner->GetTransform();
		DirectX::SimpleMath::Vector2 scale = transform->GetScale();
		float absScaleX = fabsf(scale.x);
		if (scale.x < 0.0f)
			transform->SetScale({ absScaleX, scale.y });
	}
	Move(dir);

	if (InputManager::GetInstance().GetKeyPress(VK_SPACE))
	{
		// Player 상태 확인 (이미 점프 중이면 중복 점프 방지)
		if (player->GetState() != Player::State::JUMPING)
			Jump();
	}

	UpdateAnimation(dir);
}


// 물리 기반 이동
void PlatformerController::Move(DirectX::SimpleMath::Vector2 dir)
{
	// 수평 입력이 없으면 이동 없음
	if (dir.x == 0.0f) return;

	// Object에 부착된 RigidBody 컴포넌트 가져오기
	auto rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");

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
			DirectX::SimpleMath::Vector2(dir.x * moveSpeed, gravityToScreen.y)
		);
	}
}

void PlatformerController::Jump()
{
	const float jumpPower = 20.0f;	// 점프 시 가해질 임펄스 세기

	auto player = std::make_shared<Player>();

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


void PlatformerController::UpdateAnimation(DirectX::SimpleMath::Vector2 dir)
{
	// Object에 부착된 Animator 컴포넌트 가져오기
	auto animator = GetOwner()->GetComponent<Animator>("Animator");
	// Animator가 존재하지 않으면 애니메이션 업데이트 불가
	if (animator == nullptr) return;

	// Player 상태가 Jumping일 경우 Jump 애니메이션 재생
	if (player->GetState() == Player::State::JUMPING)
	{
		animator->Play(L"Jump");
	}
	// Player 상태가 Jumping이 아니면 다른 애니메이션 재생
	else
	{
		// 좌우 이동 입력이 존재하면 Move 애니메이션 재생
		if (dir.x != 0.0f)
			animator->Play(L"Move");
		// 이동 입력이 없으면 Stand(Idle) 애니메이션 재생
		else
		{
			animator->Play(L"Stand");
		}
	}
}
