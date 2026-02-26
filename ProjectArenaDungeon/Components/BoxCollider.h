#pragma once

#include "Components/Collider.h"

// BoxCollider
// - Box2D polygon(box) shape를 생성하는 콜라이더
// - 점 판정 기능 구현
class BoxCollider final : public Collider
{
public:
	explicit BoxCollider(std::string name = "BoxCollider");

	// IsColliding
	// - Transform 기준 OBB 공간에서 점 포함 여부를 판정
	bool IsColliding(DirectX::SimpleMath::Vector2 point) override;

protected:
	// CreateShapeInternal
	// - body에 offset box shape를 생성하여 부착
	b2ShapeId CreateShapeInternal(
		b2BodyId bodyId,
		const b2ShapeDef& def,
		DirectX::SimpleMath::Vector2 ownerScale
	) override;
};