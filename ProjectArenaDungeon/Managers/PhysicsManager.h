#pragma once

#include <cstdint>

// Box2D
#include <box2d/box2d.h>

#include "_Libraries/DirectXTK/SimpleMath.h"

// PhysicsManager
// - Box2D 물리 월드를 생성/갱신하고, 센서/접촉 이벤트를 오브젝트로 전파
// - 고정 스텝 기반으로 Step을 수행
class PhysicsManager
{
	DECLARE_SINGLETON(PhysicsManager);

public:
	// Init
	// - 물리 월드를 생성하고 기본 파라미터(중력/스텝)를 초기화
	void Init();

	// Destroy
	// - 물리 월드를 해제
	void Destroy();

	// Update
	// - 고정 스텝으로 Box2D Step을 수행하고 이벤트를 처리
	void Update();

	b2WorldId GetWorldId() const { return worldId; }

	// SetGravity
	// - 월드 중력을 설정
	// - 탑다운 기본은 (0,0)으로 고정이며, 연출이나 이후 필요할 수 있는 상황에 대응한다.
	void SetGravity(DirectX::SimpleMath::Vector2 gravity);

private:
	// ProcessEvents
	// - Step 결과로 발생한 센서/접촉 이벤트를 Object의 충돌 이벤트로 전파
	void ProcessEvents();

private:
	b2WorldId worldId = b2_nullWorldId;

	float timeScale = 1.0f;
	int subStepCount = 4;

	float stepSize = 1.0f / 60.0f;
	float accumulator = 0.0f;
};