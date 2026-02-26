#pragma once
#include <box2d/box2d.h>

// 싱글턴 기반 물리 시스템 관리 클래스
// Box2D 월드 생성/파괴 관리
// 고정 시간 기반 물리 시뮬레이션 업데이트

class PhysicsManager
{
private:
	PhysicsManager();
	~PhysicsManager() = default;

public:
	// 싱글턴 복사/이동 금지
	PhysicsManager(const PhysicsManager& other) = delete;
	PhysicsManager& operator=(const PhysicsManager& other) = delete;
	PhysicsManager(PhysicsManager&& other) = delete;
	PhysicsManager& operator=(PhysicsManager&& other) = delete;

	// 싱글턴 인스턴스 반환
	static PhysicsManager& GetInstance()
	{
		static PhysicsManager instance;
		return instance;
	}

public:
	// 물리 월드 생성 및 초기화
	void Init();
	//물리 월드 파괴 및 리소스 해제
	void Destroy();
	// 물리 시뮬레이션 업데이트
	// TimeManager deltaTime 누적
	// 고정 스텝(stepCount)만큼 Box2D Step 수행
	void Update();
	
	// Box2D 월드 ID 반환
	b2WorldId GetWorldId() const { return worldId; }

	// Box2D 월드의 gravity를 갱신하며, 이후 물리 시뮬레이션에 적용
	void SetGravity(DirectX::SimpleMath::Vector2 gravity);

private:
	// Box2D에서 발생한 Sensor / Contact 이벤트를 처리하는 내부 함수
	// 각 Shape의 UserData에 등록된 Collider를 통해 Object -> Component 순으로 충돌 이벤트를 전달
	void ProcessEvents();

	b2WorldId worldId = b2_nullWorldId; // Box2D 물리 월드 ID
	float timeScale = 1.0f;		// 물리 시뮬레이션 속도 배율
	int subStepCount = 4;		// 프레임당 최대 물리 스텝 횟수 (고정 시간동안 반복 수)

	float stepSize = 1.0f / 144.0f; // 물리 고정 시간 간격
	float timeAccumulator = 0.0f;	// 누적 시간 (초)
};