#pragma once

#include <cstdint>

// Box2D
#include <box2d/box2d.h>

// DirectXTK SimpleMath
#include "_Libraries/DirectXTK/SimpleMath.h"

#include "Config/ConstantValues.h"

// CollisionLayer
// - Box2D 필터(categoryBits/maskBits)에 사용하는 충돌 레이어 정의
// - Environment는 본 프로젝트에서 벽으로 대표되는 고정 오브젝트 등 월드 정적 요소
enum class CollisionLayer : std::uint32_t
{
	Default = 0x0001,
	Environment = 0x0002,
	Player = 0x0004,
	Enemy = 0x0008,
	Bullet = 0x0010,
	UI = 0x0020,
};

// operator | 오버로딩
inline std::uint32_t operator|(CollisionLayer a, CollisionLayer b)
{
	return static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b);
}

inline std::uint32_t operator|(std::uint32_t a, CollisionLayer b)
{
	return a | static_cast<std::uint32_t>(b);
}

namespace PhysicsUtils
{
// ScreenToWorld
// - 화면 좌표(픽셀)를 물리 좌표(미터)로 변환
inline b2Vec2 ScreenToWorld(const DirectX::SimpleMath::Vector2& screenPos)
{
	return b2Vec2{ screenPos.x / PTM_RATIO, screenPos.y / PTM_RATIO };
}

// WorldToScreen
// - 물리 좌표(미터)를 화면 좌표(픽셀)로 변환
inline DirectX::SimpleMath::Vector2 WorldToScreen(const b2Vec2& worldPos)
{
	return DirectX::SimpleMath::Vector2{ worldPos.x * PTM_RATIO, worldPos.y * PTM_RATIO };
}
}