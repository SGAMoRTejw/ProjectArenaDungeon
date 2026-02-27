#include "pch.h"
#include "Components/Collider.h"

#include <cmath>
#include "Config/ConstantValues.h"

#include "Objects/Object.h"

#include "Components/Transform.h"
#include "Components/RigidBody.h"

using Vector2 = DirectX::SimpleMath::Vector2;

namespace
{
	constexpr float MIN_PIXEL_SIZE = 0.001f;
}

Collider::Collider(std::string name)
	: Component(std::move(name))
{
}

Collider::~Collider()
{
	if (b2Shape_IsValid(shapeId))
	{
		b2DestroyShape(shapeId, true);
		shapeId = b2_nullShapeId;
	}
}

void Collider::Awake()
{
	RefreshShape();
}

void Collider::Update()
{
	if (!owner)
		return;

	const auto* tr = owner->GetTransform();
	if (!tr)
		return;

	const Vector2 currentScale = tr->GetScale();

	// NOTE: DistanceSquared는 제곱 값이므로 epsilonSq로 비교한다.
	if (Vector2::DistanceSquared(currentScale, lastScale) > epsilonSq)
	{
		RefreshShape();
	}
}

void Collider::SetOffset(Vector2 value)
{
	if (offset != value)
	{
		offset = value;
		RefreshShape();
	}
}

void Collider::SetCollisionLayer(CollisionLayer value)
{
	if (layer == value)
		return;

	layer = value;
	ApplyFilter();
}

void Collider::SetCollisionMask(uint32_t value)
{
	if (mask == value)
		return;

	mask = value;
	ApplyFilter();
}

void Collider::SetIsSensor(bool value)
{
	if (isSensor == value)
		return;

	isSensor = value;
	RefreshShape();
}

void Collider::RefreshShape()
{
	if (!owner)
		return;

	auto rb = owner->GetComponent<RigidBody>("RigidBody");
	if (!rb || !b2Body_IsValid(rb->GetBodyId()))
		return;

	const auto* tr = owner->GetTransform();
	if (!tr)
		return;

	const Vector2 scale = tr->GetScale();

	// NOTE: 너무 작은 스케일은 제외한다.
	if (std::abs(scale.x) < MIN_PIXEL_SIZE || std::abs(scale.y) < MIN_PIXEL_SIZE)
		return;

	if (b2Shape_IsValid(shapeId))
	{
		// NOTE: shape 파괴로 인해 겹침 상태가 해제될 수 있으므로,
		//       기존 AABB와 겹치던 대상에게 Exit를 전파한 뒤 shape를 파괴한다.
		const b2AABB aabb = b2Shape_GetAABB(shapeId);
		b2World_OverlapAABB(PHYSICS.GetWorldId(), aabb, b2DefaultQueryFilter(), NotifyExitCallback, this);

		b2DestroyShape(shapeId, true);
		shapeId = b2_nullShapeId;
	}

	lastScale = scale;

	b2ShapeDef def = b2DefaultShapeDef();
	def.density = 1.0f;
	def.material.friction = 0.5f;
	def.material.restitution = 0.1f;

	def.userData = this;

	def.isSensor = isSensor;
	def.enableSensorEvents = true;
	if (!isSensor)
	{
		def.enableContactEvents = true;
	}

	def.filter.categoryBits = static_cast<uint32_t>(layer);
	def.filter.maskBits = mask;

	shapeId = CreateShapeInternal(rb->GetBodyId(), def, scale);

	b2Body_SetAwake(rb->GetBodyId(), true);
}

bool Collider::NotifyExitCallback(b2ShapeId otherShapeId, void* context)
{
	auto* me = static_cast<Collider*>(context);
	if (!me || !me->owner)
		return true;

	if (!b2Shape_IsValid(otherShapeId))
		return true;

	auto* other = static_cast<Collider*>(b2Shape_GetUserData(otherShapeId));
	
	if (!other)
		return true;

	// 상대에게 나를 알림
	if (auto* otherOwner = other->GetOwner())
		otherOwner->OnCollisionExit(me);

	// 나에게도 상대를 알림
	if (auto* myOwner = me->GetOwner())
		myOwner->OnCollisionExit(other);

	return true;
}

void Collider::ApplyFilter() const
{
	if (!b2Shape_IsValid(shapeId))
		return;

	b2Filter filter = b2DefaultFilter();
	filter.categoryBits = static_cast<uint32_t>(layer);
	filter.maskBits = mask;
	filter.groupIndex = 0;

	b2Shape_SetFilter(shapeId, filter);
}