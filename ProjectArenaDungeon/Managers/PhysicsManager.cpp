#include "pch.h"
#include "PhysicsManager.h"

//#include "Components/Collider.h"
#include "Objects/Object.h"

using Vector2 = DirectX::SimpleMath::Vector2;

PhysicsManager::PhysicsManager() {}

void PhysicsManager::Init()
{
	if (b2World_IsValid(worldId))
	{
		Destroy();
	}

	b2WorldDef worldDef = b2DefaultWorldDef();

	// NOTE: 탑다운 기본은 중력 미사용.
	worldDef.gravity = { 0.0f, 0.0f };

	worldId = b2CreateWorld(&worldDef);
}

void PhysicsManager::Destroy()
{
	if (b2World_IsValid(worldId))
	{
		b2DestroyWorld(worldId);
		worldId = b2_nullWorldId;
	}

	accumulator = 0.0f;
}

void PhysicsManager::Update()
{
	if (!b2World_IsValid(worldId))
		return;

	float dt = TIME.GetDeltaTime();

	// NOTE: 프레임 드랍 시 폭주 방지.
	if (dt > 0.25f)
		dt = 0.25f;

	accumulator += dt;

	while (accumulator >= stepSize)
	{
		b2World_Step(worldId, stepSize * timeScale, subStepCount);

		ProcessEvents();

		accumulator -= stepSize;
	}
}

void PhysicsManager::SetGravity(Vector2 gravity)
{
	if (!b2World_IsValid(worldId))
		return;

	b2World_SetGravity(worldId, { gravity.x, gravity.y });
}

void PhysicsManager::ProcessEvents()
{
	// NOTE: Sensor 이벤트 처리
	// - Collider 컴포넌트 생성까지 빌드 오류 방지를 위해 임시 주석 처리
	//const b2SensorEvents sEvents = b2World_GetSensorEvents(worldId);
	//
	//auto HandleSensor = [](b2ShapeId sensorId, b2ShapeId visitorId, bool isBegin)
	//	{
	//		if (!b2Shape_IsValid(sensorId) || !b2Shape_IsValid(visitorId))
	//			return;
	//
	//		auto* colSensor = static_cast<Collider*>(b2Shape_GetUserData(sensorId));
	//		auto* colVisitor = static_cast<Collider*>(b2Shape_GetUserData(visitorId));
	//
	//		if (!colSensor || !colVisitor)
	//			return;
	//
	//		if (auto* owner = colSensor->GetOwner())
	//			isBegin ? owner->OnCollisionEnter(colVisitor) : owner->OnCollisionExit(colVisitor);
	//
	//		if (auto* visitor = colVisitor->GetOwner())
	//			isBegin ? visitor->OnCollisionEnter(colSensor) : visitor->OnCollisionExit(colSensor);
	//	};
	//
	//for (int i = 0; i < sEvents.beginCount; ++i)
	//	HandleSensor(sEvents.beginEvents[i].sensorShapeId, sEvents.beginEvents[i].visitorShapeId, true);
	//
	//for (int i = 0; i < sEvents.endCount; ++i)
	//	HandleSensor(sEvents.endEvents[i].sensorShapeId, sEvents.endEvents[i].visitorShapeId, false);
	//
	//// NOTE: Contact 이벤트 처리
	//const b2ContactEvents cEvents = b2World_GetContactEvents(worldId);
	//
	//auto HandleContact = [](b2ShapeId idA, b2ShapeId idB, bool isBegin)
	//	{
	//		if (!b2Shape_IsValid(idA) || !b2Shape_IsValid(idB))
	//			return;
	//
	//		auto* colA = static_cast<Collider*>(b2Shape_GetUserData(idA));
	//		auto* colB = static_cast<Collider*>(b2Shape_GetUserData(idB));
	//
	//		if (!colA || !colB)
	//			return;
	//
	//		if (auto* ownerA = colA->GetOwner())
	//			isBegin ? ownerA->OnCollisionEnter(colB) : ownerA->OnCollisionExit(colB);
	//
	//		if (auto* ownerB = colB->GetOwner())
	//			isBegin ? ownerB->OnCollisionEnter(colA) : ownerB->OnCollisionExit(colA);
	//	};
	//
	//for (int i = 0; i < cEvents.beginCount; ++i)
	//	HandleContact(cEvents.beginEvents[i].shapeIdA, cEvents.beginEvents[i].shapeIdB, true);
	//
	//for (int i = 0; i < cEvents.endCount; ++i)
	//	HandleContact(cEvents.endEvents[i].shapeIdA, cEvents.endEvents[i].shapeIdB, false);
}