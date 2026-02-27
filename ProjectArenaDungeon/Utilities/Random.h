#pragma once
#include "_Libraries/DirectXTK/SimpleMath.h"

using Vector2 = DirectX::SimpleMath::Vector2;
using Color = DirectX::SimpleMath::Color;

namespace Random
{
// Init
// - random_device 기반 시드로 난수 생성기를 초기화
//
// NOTE:
// - 본 프로젝트에서는 GameInstance::Init()에서 1회 호출하는 것을 전제로 한다.
void Init();

// GetFloatZeroToOne
// - [0.0f, 1.0f] 구간의 float 난수를 반환
float GetFloatZeroToOne();

// Range
// - min, max 사이의 난수 생성 후 반환
// - min > max 입력이 들어오면 swap 후 처리
float Range(float min, float max);
int   Range(int min, int max);

// GetColor
// - R,G,B는 [0,1] 난수, A는 1.0으로 고정한 Color 반환
Color GetColor();

// GetVector2
// - x, y 각각 [min,max] 난수로 구성한 Vector2 반환
Vector2 GetVector2(float min, float max);
}