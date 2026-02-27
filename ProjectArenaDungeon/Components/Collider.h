#pragma once

#include <cstdint>

#include <box2d/box2d.h>
#include "_Libraries/DirectXTK/SimpleMath.h"

#include "Components/Component.h"
#include "Utilities/PhysicsUtility.h"

// Collider
// - Box2D shape를 RigidBody(body)에 부착하는 컴포넌트 기반 클래스
// - scale/offset/sensor/filter 변경 시 shape를 재생성하여 동기화
class Collider : public Component
{
public:
	explicit Collider(std::string name = "Collider");
	~Collider() override;

	// Awake
	// - 현재 Transform/RigidBody 상태를 기준으로 shape를 생성
	void Awake() override;

	// Update
	// - Transform scale 변화가 감지되면 shape를 갱신
	void Update() override;

	// IsColliding
	// - 점 기반 충돌 판정
	virtual bool IsColliding(DirectX::SimpleMath::Vector2 point) = 0;

	// SetOffset
	// - body 로컬 공간에서 shape 중심 오프셋(픽셀 단위)
	// - 값 변경 시 shape를 재생성
	void SetOffset(DirectX::SimpleMath::Vector2 value);

	DirectX::SimpleMath::Vector2 GetOffset() const { return offset; }

	// SetCollisionLayer / SetCollisionMask
	// - Box2D 필터(category/mask)를 설정
	// - shape가 이미 존재하면 필터를 반영
	void SetCollisionLayer(CollisionLayer value);
	void SetCollisionMask(uint32_t value);

	// SetIsSensor
	// - sensor 여부를 변경
	// - shape 생성 시 세팅 전용으로 사용
	void SetIsSensor(bool value);

	bool IsSensor() const { return isSensor; }

	b2ShapeId GetShapeId() const { return shapeId; }

protected:
	// CreateShapeInternal
	// - 파생 클래스가 도형 형태(box/circle 등)를 구성하고 shape를 생성
	virtual b2ShapeId CreateShapeInternal(
		b2BodyId bodyId,
		const b2ShapeDef& def,
		DirectX::SimpleMath::Vector2 ownerScale
	) = 0;

	// RefreshShape
	// - 현재 Transform/RigidBody/필터/센서 설정을 기준으로 shape를 재생성
	void RefreshShape();

	// ApplyFilter
	// - 현재 layer/mask를 기존 shape에 반영
	void ApplyFilter() const;

	// NotifyExitCallback
	// - shape 파괴 직전, 기존 AABB와 겹치던 대상에게 Exit를 전파하기 위한 콜백
	static bool NotifyExitCallback(b2ShapeId otherShapeId, void* context);

protected:
	b2ShapeId shapeId = b2_nullShapeId;

	bool isSensor = false;

	DirectX::SimpleMath::Vector2 offset = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	DirectX::SimpleMath::Vector2 lastScale = DirectX::SimpleMath::Vector2(-999.0f, -999.0f);

	CollisionLayer layer = CollisionLayer::Default;
	uint32_t mask = 0xFFFFFFFF;
};