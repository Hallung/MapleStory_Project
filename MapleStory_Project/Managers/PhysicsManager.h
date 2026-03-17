#pragma once
#include "Components/Collider.h"
#include <box2d/box2d.h>

//===============================
// Raycast 결과 구조체
// Ray가 충돌했을 때의 정보를 저장
//===============================
struct RaycastHit
{
	bool hit = false;						// Ray가 어떤 Collider와 충돌했는지 여부

	Collider* collider = nullptr;			// 충돌한 Collider

	DirectX::SimpleMath::Vector2 point;		// 충돌 지점 (Screen 좌표)
	DirectX::SimpleMath::Vector2 normal;	// 충돌 표면의 법선 벡터

	float fraction = 0.0f; // ray 진행 비율 (0 ~ 1) (0 = 시작점, 1 = Ray 끝)
};

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

	//==================================================
	// 지정한 위치에서 특정 방향으로 Ray를 발사하여
	// 가장 가까운 충돌 객체를 탐색하는 함수
	//==================================================
	RaycastHit Raycast(
		const DirectX::SimpleMath::Vector2& origin,	// Ray 시작 위치 (Screen 좌표)
		const DirectX::SimpleMath::Vector2& dir,	// Ray 방향 벡터
		float distance,								// Ray 길이
		uint32_t layerMask							// 충돌 검사할 레이어 마스크
	);

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