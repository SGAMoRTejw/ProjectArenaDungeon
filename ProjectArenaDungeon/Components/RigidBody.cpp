#include "pch.h"
#include "Components/RigidBody.h"

#include "Objects/Object.h"
#include "Components/Transform.h"

#include "Managers/PhysicsManager.h"
#include "Utilities/PhysicsUtility.h"

using Vector2 = DirectX::SimpleMath::Vector2;

RigidBody::RigidBody(BodyType type, std::string name)
	: Component(std::move(name)), type(type)
{
	bodyDef = b2DefaultBodyDef();
	bodyDef.type = static_cast<b2BodyType>(type);

	// NOTE: 생성 이후 옵션 변경이 있을 수 있으므로 bodyDef에도 값을 유지한다.
}

RigidBody::~RigidBody()
{
	if (b2Body_IsValid(bodyId))
	{
		b2DestroyBody(bodyId);
		bodyId = b2_nullBodyId;
	}
}

void RigidBody::Awake()
{
	if (!owner)
		return;

	const b2WorldId worldId = PHYSICS.GetWorldId();
	if (!b2World_IsValid(worldId))
		return;

	auto* tr = owner->GetTransform();
	if (!tr)
		return;

	bodyDef.position = PhysicsUtility::ScreenToWorld(tr->GetPosition());
	bodyDef.rotation = b2MakeRot(-tr->GetRotationRadian());
	bodyDef.userData = owner;

	bodyId = b2CreateBody(worldId, &bodyDef);

	// NOTE: bodyDef에 저장된 옵션을 실제 바디에도 반영한다.
	if (b2Body_IsValid(bodyId))
	{
		b2Body_SetLinearDamping(bodyId, bodyDef.linearDamping);
		b2Body_SetAngularDamping(bodyId, bodyDef.angularDamping);
		b2Body_SetFixedRotation(bodyId, bodyDef.fixedRotation);
	}
}

void RigidBody::Update()
{
	if (!owner)
		return;

	if (!b2Body_IsValid(bodyId))
		return;

	// NOTE: Static 바디는 물리 결과로 Transform이 변하지 않으므로 동기화하지 않는다.
	if (bodyDef.type == b2_staticBody)
		return;

	const b2Vec2 pos = b2Body_GetPosition(bodyId);
	const float angle = b2Rot_GetAngle(b2Body_GetRotation(bodyId));

	auto* tr = owner->GetTransform();
	if (!tr)
		return;

	tr->SetPosition(PhysicsUtility::WorldToScreen(pos));
	tr->SetRotationRadian(-angle);
}

void RigidBody::SetPosition(const Vector2& position)
{
	if (!b2Body_IsValid(bodyId))
		return;

	const b2Vec2 pos = PhysicsUtility::ScreenToWorld(position);
	const b2Rot rot = b2Body_GetRotation(bodyId);

	b2Body_SetTransform(bodyId, pos, rot);
	b2Body_SetAwake(bodyId, true);
}

void RigidBody::SetRotationRadian(float radian)
{
	bodyDef.rotation = b2MakeRot(-radian);

	if (!b2Body_IsValid(bodyId))
		return;

	const b2Vec2 pos = b2Body_GetPosition(bodyId);

	b2Body_SetTransform(bodyId, pos, b2MakeRot(-radian));

	// NOTE: 강제 회전 변경 시 현재 각속도는 끊어주는 편이 안전하다.
	b2Body_SetAngularVelocity(bodyId, 0.0f);

	if (owner && owner->GetTransform())
		owner->GetTransform()->SetRotationRadian(radian);

	b2Body_SetAwake(bodyId, true);
}

void RigidBody::SetVelocity(const Vector2& velocity)
{
	if (!b2Body_IsValid(bodyId))
		return;

	b2Body_SetLinearVelocity(bodyId, PhysicsUtility::ScreenToWorld(velocity));
}

void RigidBody::SetDamping(float linear, float angular)
{
	bodyDef.linearDamping = linear;
	bodyDef.angularDamping = angular;

	if (!b2Body_IsValid(bodyId))
		return;

	b2Body_SetLinearDamping(bodyId, linear);
	b2Body_SetAngularDamping(bodyId, angular);
}

void RigidBody::SetFixedRotation(bool fixed)
{
	bodyDef.fixedRotation = fixed;

	if (!b2Body_IsValid(bodyId))
		return;

	b2Body_SetFixedRotation(bodyId, fixed);
}