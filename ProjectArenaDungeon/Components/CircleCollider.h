#pragma once

#include "Components/Collider.h"

// CircleCollider
// - Box2D circle shape를 생성하는 콜라이더
// - 점 판정 기능 구현
class CircleCollider final : public Collider
{
public:
	explicit CircleCollider(std::string name = "CircleCollider");

	// IsColliding
	// - Transform 기준 원(스케일 기반 반지름) 포함 여부를 판정
	bool IsColliding(DirectX::SimpleMath::Vector2 point) override;

protected:
	// CreateShapeInternal
	// - body에 offset circle shape를 생성하여 부착
	b2ShapeId CreateShapeInternal(
		b2BodyId bodyId,
		const b2ShapeDef& def,
		DirectX::SimpleMath::Vector2 ownerScale
	) override;
};