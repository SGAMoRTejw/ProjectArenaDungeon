#include "pch.h"
#include "Components/BoxCollider.h"

#include <cmath>
#include "Config/ConstantValues.h"

#include "Objects/Object.h"

#include "Components/Transform.h"

#include "Utilities/PhysicsUtility.h"

using Vector2 = DirectX::SimpleMath::Vector2;

BoxCollider::BoxCollider(std::string name)
	: Collider(std::move(name))
{
}

bool BoxCollider::IsColliding(Vector2 point)
{
	if (!owner)
		return false;

	const auto* tr = owner->GetTransform();
	if (!tr)
		return false;

	const Vector2 scale = tr->GetScale();

	const Vector2 center =
		tr->GetPosition()
		+ tr->GetRight() * GetOffset().x
		+ tr->GetUp() * GetOffset().y;

	const Vector2 dist = point - center;

	const Vector2 localPoint(dist.Dot(tr->GetRight()), dist.Dot(tr->GetUp()));
	const Vector2 halfScale(std::abs(scale.x) * 0.5f, std::abs(scale.y) * 0.5f);

	return (std::abs(localPoint.x) <= halfScale.x) && (std::abs(localPoint.y) <= halfScale.y);
}

b2ShapeId BoxCollider::CreateShapeInternal(b2BodyId bodyId, const b2ShapeDef& def, Vector2 ownerScale)
{
	const float hx = (std::abs(ownerScale.x) * 0.5f) / PTM_RATIO;
	const float hy = (std::abs(ownerScale.y) * 0.5f) / PTM_RATIO;

	const b2Vec2 center = PhysicsUtility::ScreenToWorld(GetOffset());
	const b2Polygon box = b2MakeOffsetBox(hx, hy, center, b2MakeRot(0.0f));

	return b2CreatePolygonShape(bodyId, &def, &box);
}