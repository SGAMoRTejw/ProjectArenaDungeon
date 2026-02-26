#include "pch.h"
#include "Components/CircleCollider.h"

#include <algorithm>
#include <cmath>
#include "Config/ConstantValues.h"

#include "Objects/Object.h"

#include "Components/Transform.h"

#include "Utilities/PhysicsUtility.h"

using Vector2 = DirectX::SimpleMath::Vector2;

CircleCollider::CircleCollider(std::string name)
	: Collider(std::move(name))
{
}

bool CircleCollider::IsColliding(Vector2 point)
{
	auto* ownerObj = GetOwner();
	if (!ownerObj) return false;

	const auto* tr = ownerObj->GetTransform();
	if (!tr) return false;

	const Vector2 center =
		tr->GetPosition()
		+ tr->GetRight() * GetOffset().x
		+ tr->GetUp() * GetOffset().y;

	const auto scale = tr->GetScale();

	// NOTE: 원 반지름은 scale의 큰 축 기준(픽셀 단위)으로 계산한다.
	const float radius = std::max(std::abs(scale.x), std::abs(scale.y)) * 0.5f;

	const auto delta = point - center;
	const float distSq = delta.LengthSquared();
	const float radiusSq = radius * radius;

	return distSq < radiusSq;
}

b2ShapeId CircleCollider::CreateShapeInternal(b2BodyId bodyId, const b2ShapeDef& def, Vector2 ownerScale)
{
	const float radius = (std::max(std::abs(ownerScale.x), std::abs(ownerScale.y)) * 0.5f) / PTM_RATIO;

	const b2Vec2 center = PhysicsUtility::ScreenToWorld(GetOffset());
	const b2Circle circle{ center, radius };

	return b2CreateCircleShape(bodyId, &def, &circle);
}