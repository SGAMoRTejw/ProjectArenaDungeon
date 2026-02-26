#pragma once
// DirectXTK SimpleMath
#include "_Libraries/DirectXTK/SimpleMath.h"

// Constants
constexpr float epsilon = 1e-5f;
constexpr DirectX::SimpleMath::Vector2 VEC2_ZERO = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
constexpr DirectX::SimpleMath::Vector2 VEC2_ONE = DirectX::SimpleMath::Vector2(1.0f, 1.0f);
// NOTE: 1m 당 픽셀 스케일. 값이 바뀌면 전체 물리 체감이 변경
constexpr float PTM_RATIO = 50.0f;