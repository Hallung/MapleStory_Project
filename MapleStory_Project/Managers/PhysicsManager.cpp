#include "stdafx.h"
#include "PhysicsManager.h"
#include "TimeManager.h"
#include "Objects/Object.h"
#include "Utilities/PhysicsUtils.h"

PhysicsManager::PhysicsManager() {}

void PhysicsManager::Init()
{
	// 기존 월드가 존재하면 먼저 파괴
	if (b2World_IsValid(worldId)) Destroy();

	// Box2D 월드 정의 생성
	b2WorldDef worldDef = b2DefaultWorldDef();
	// 중력 설정 (-Y축, 아래 방향)
	worldDef.gravity = { 0.0f, -30.0f };
	// 물리 월드 생성
	worldId = b2CreateWorld(&worldDef);
}

void PhysicsManager::Destroy()
{
	// 유효한 월드가 있을 경우 파괴
	if (b2World_IsValid(worldId))
	{
		b2DestroyWorld(worldId);
		worldId = b2_nullWorldId;
	}
}

void PhysicsManager::Update()
{
	// 유효한 월드가 있을 경우에만 실행
	if (b2World_IsValid(worldId))
	{
		// 프레임 deltaTime 가져오기
		float deltaTime = TimeManager::GetInstance().GetDeltaTime();

		// 과도한 프레임 시간 제한 (물리 폭주 방지)
		if (deltaTime > 0.25f) deltaTime = 0.25f;

		// 누적 시간에 프레임 시간 추가
		timeAccumulator += deltaTime;

		// 고정 시간 만큼 물리 스텝 반복 수행
		while (timeAccumulator >= stepSize)
		{
			// 월드가 유효한 경우에만 시뮬레이션 진행
			if (b2World_IsValid(worldId))
			{
				// Box2D 물리 스텝 수행
				// stepSize 에 timeScale을 곱하여 슬로우 모션 / 일시정지 지원
				// stepCount : 서브 스텝 반복 횟수 (정확도 향상)
				b2World_Step(worldId, stepSize * timeScale, subStepCount);

				// 물리 Step 이후 충돌 이벤트 처리
				// Box2D가 감지한 Sensor / Contact 이벤트를 Collider -> Object -> Component로 전달
				ProcessEvents();

				// 사용된 시간 차감
				timeAccumulator -= stepSize;
			}
		}
	}
}

// Box2D 월드의 중력 값을 설정, 이미 생성된 모든 RigidBody에 적용되며 다음 물리 Step부터 반영
void PhysicsManager::SetGravity(DirectX::SimpleMath::Vector2 gravity)
{
	if (b2World_IsValid(worldId))
		b2World_SetGravity(worldId, { gravity.x, gravity.y });
}

RaycastHit PhysicsManager::Raycast(const DirectX::SimpleMath::Vector2& origin, const DirectX::SimpleMath::Vector2& dir, float distance, uint32_t layerMask)
{
	RaycastHit hit;
	hit.hit = false;	// 기본값: 충돌 없음

	// Ray 방향 정규화(방향 벡터 크기를 1로 만들어 distance와 정확히 곱해지도록 함)
	DirectX::SimpleMath::Vector2 ndir = dir;
	ndir.Normalize();

	// Screen 좌표 → Box2D World 좌표 변환
	b2Vec2 p1 = PhysicsUtils::ScreenToWorld(origin);

	// Ray 이동 벡터 계산
	b2Vec2 translation = { ndir.x * distance, ndir.y * distance };
	
	// Raycast 필터 설정
	// (castegoryBits : 필터링 단계에서 불필요한 충돌 검사 방지)
	// (maskBits : 어떤 Collision Layer를 검사할지 결정)
	b2QueryFilter filter = b2DefaultQueryFilter();
	filter.categoryBits = static_cast<uint32_t>(CollisionLayer::Raycast);
	filter.maskBits = layerMask;
	
	// Box2D Raycast 실행
	b2RayResult result = b2World_CastRayClosest(worldId, p1, translation, filter);
	
	// Ray가 무언가에 충돌한 경우
	if (result.hit)
	{
		// Shape에 저장해둔 UserData → 엔진 Collider 복원
		Collider* col = (Collider*)b2Shape_GetUserData(result.shapeId);

		// Collider 존재 + LayerMask 검사 통과 확인
		if (col && (layerMask & (uint32_t)col->GetCollisionLayer()))
		{
			hit.hit = true;
			hit.collider = col;
			// World → Screen 좌표 변환
			hit.point = PhysicsUtils::WorldToScreen(result.point);
			// 충돌 표면의 법선 벡터
			hit.normal = { result.normal.x, result.normal.y };
			// Ray 진행률 (충돌 위치 비율)
			hit.fraction = result.fraction;

			// 디버깅용 로그(필요 시 활성화)
			//std::cout << "★ GROUND HIT! fraction: " << result.fraction
			//	<< " at (" << hit.point.x << ", " << hit.point.y << ")\n";
		}
	}
	else
	{
		// Ray가 아무것도 맞추지 못한 경우

		// 디버깅용 로그(필요 시 활성화)
		//std::cout << "No hit - ray missed ground\n";
		//std::cout << "[DEBUG Ray] p1: " << p1.x << ", " << p1.y
		//	<< " → p2: " << p1.x + translation.x << ", " << p1.y + translation.y << '\n';
	}

	return hit;
}

// Box2D에서 발생한 Sensor / Contact 이벤트를 처리하는 내부 함수
void PhysicsManager::ProcessEvents()
{
	//=====================================================
	// Sensor 이벤트 처리
	// Sensor는 물리 충돌은 하지 않고 겹침만 감지하는 Collider
	//=====================================================
	b2SensorEvents sEvents = b2World_GetSensorEvents(worldId);

	// Sensor 이벤트 공통 처리 람다 헬퍼 함수
	// sensor : Sensor Collider
	// visitor : Sensor에 들어온 Collider
	// isBegin : true = 충돌 시작, false = 충돌 종료
	auto HandleSensor = [](b2ShapeId sensorId, b2ShapeId visitorId, bool isBegin)
		{
			// Shape 유효성 검사
			if (b2Shape_IsValid(sensorId) == false || b2Shape_IsValid(visitorId) == false)
				return;

			// Shape UserData에 저장된 Collider 가져오기
			auto colSensor = (Collider*)b2Shape_GetUserData(sensorId);
			auto colVisitor = (Collider*)b2Shape_GetUserData(visitorId);

			if (colSensor && colVisitor)
			{
				// Sensor 소유 Object 이벤트 전달
				if (auto owner = colSensor->GetOwner())
					isBegin ? owner->OnCollisionEnter(colVisitor) : owner->OnCollisionExit(colVisitor);

				// Visitor 소유 Object 이벤트 전달
				if (auto visitor = colVisitor->GetOwner())
					isBegin ? visitor->OnCollisionEnter(colSensor) : visitor->OnCollisionExit(colSensor);
			}
		};

	// beginEvents : 이번 물리 Step에서 새롭게 충돌이 시작된 이벤트 목록
	// beginCount : 충돌 시작 이벤트 개수
	// -> 각 충돌 쌍에 대해 OnCollisionEnter 호출
	for (int i = 0; i < sEvents.beginCount; ++i)
		HandleSensor(sEvents.beginEvents[i].sensorShapeId, sEvents.beginEvents[i].visitorShapeId, true);

	// endEvents : 이번 물리 Step에서 충돌이 종료된 이벤트 목록
	// endCount : 충돌 종료 이벤트 개수
	// -> 각 충돌 쌍에 대해 OnCollisionExit 호출
	for (int i = 0; i < sEvents.endCount; ++i)
		HandleSensor(sEvents.endEvents[i].sensorShapeId, sEvents.endEvents[i].visitorShapeId, false);

	//===================================================
	// Contact 이벤트 처리
	// Contact는 물리 충돌 및 물리 반응이 발생하는 Collider
	//===================================================
	b2ContactEvents cEvents = b2World_GetContactEvents(worldId);

	// Contact 이벤트 공통 처리 람다 헬퍼 함수
	// idA : 충돌한 첫 번째 Collider Shape
	// idB : 충돌한 두 번째 Collider Shape
	// isBegin : true = 충돌 시작, false = 충돌 종료
	auto HandleContact = [](b2ShapeId idA, b2ShapeId idB, bool isBegin)
		{
			// Shape 유효성 검사
			if (b2Shape_IsValid(idA) == false || b2Shape_IsValid(idB) == false)
				return;

			// Shape UserData에 저장된 Collider 가져오기
			auto colA = (Collider*)b2Shape_GetUserData(idA);
			auto colB = (Collider*)b2Shape_GetUserData(idB);

			if (colA && colB)
			{
				// Collider A 소유 Object 이벤트 전달
				if (auto ownerA = colA->GetOwner())
					isBegin ? ownerA->OnCollisionEnter(colB) : ownerA->OnCollisionExit(colB);

				// Collider B 소유 Object 이벤트 전달
				if (auto ownerB = colB->GetOwner())
					isBegin ? ownerB->OnCollisionEnter(colA) : ownerB->OnCollisionExit(colA);
			}
		};

	// beginEvents : 이번 물리 Step에서 새롭게 충돌이 시작된 이벤트 목록
	// beginCount : 충돌 시작 이벤트 개수
	// -> 각 충돌 쌍에 대해 OnCollisionEnter 호출
	for (int i = 0; i < cEvents.beginCount; ++i)
		HandleContact(cEvents.beginEvents[i].shapeIdA, cEvents.beginEvents[i].shapeIdB, true);

	// endEvents : 이번 물리 Step에서 충돌이 종료된 이벤트 목록
	// endCount : 충돌 종료 이벤트 개수
	// -> 각 충돌 쌍에 대해 OnCollisionExit 호출
	for (int i = 0; i < cEvents.endCount; ++i)
		HandleContact(cEvents.endEvents[i].shapeIdA, cEvents.endEvents[i].shapeIdB, false);
}
