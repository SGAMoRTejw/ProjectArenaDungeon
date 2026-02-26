#pragma once

#include <string>

// Box2D
#include <box2d/box2d.h>

// DirectXTK SimpleMath
#include "_Libraries/DirectXTK/SimpleMath.h"

#include "Components/Component.h"

enum class BodyType
{
	Static = b2_staticBody,
	Dynamic = b2_dynamicBody,
	Kinematic = b2_kinematicBody,
};

// RigidBody
// - Box2D 바디를 생성하고 Transform과 물리 상태를 동기화
// - 픽셀 좌표계를 물리 좌표계(미터)로 변환하여 월드에 반영
class RigidBody : public Component
{
public:
	explicit RigidBody(BodyType type = BodyType::Dynamic, std::string name = "RigidBody");
	~RigidBody() override;

	// Awake
	// - 현재 Transform 기준으로 Box2D 바디를 생성
	// - 월드가 유효하지 않으면 생성하지 않음
	void Awake() override;

	// Update
	// - Box2D 바디의 위치/회전을 Transform에 반영
	// - Static 바디는 스킵
	void Update() override;

	// SetPosition
	// - 바디 위치를 픽셀 좌표로 지정하고 월드 변환으로 적용
	void SetPosition(const DirectX::SimpleMath::Vector2& position);

	// SetRotationRadian
	// - 바디 회전을 라디안으로 지정
	// - 엔진 회전 방향 정책에 맞춰 Box2D 각도 부호를 변환
	void SetRotationRadian(float radian);

	// SetVelocity
	// - 선속도를 픽셀/초 기준으로 지정하고 월드 변환으로 적용
	void SetVelocity(const DirectX::SimpleMath::Vector2& velocity);

	// SetDamping
	// - 선/각 감쇠를 설정하고, 생성된 바디에 반영
	void SetDamping(float linear, float angular);

	// SetFixedRotation
	// - 회전 고정 여부를 설정하고, 생성된 바디에 반영
	void SetFixedRotation(bool fixed);

	b2BodyId GetBodyId() const { return bodyId; }
	BodyType GetBodyType() const { return type; }

private:
	BodyType type = BodyType::Dynamic;

	b2BodyId bodyId = b2_nullBodyId;
	b2BodyDef bodyDef{};
};