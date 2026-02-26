#pragma once

#include "Components/Component.h"

#include <memory>
#include <cmath>

// DirectXTK SimpleMath
#include "_Libraries/DirectXTK/SimpleMath.h"

class WorldBuffer;

// Transform
// - 오브젝트의 위치/회전/스케일을 보관하고, 월드 행렬을 계산
// - 변경이 있을 때만 갱신하도록 dirty 플래그 기반으로 업데이트
class Transform : public Component
{
public:
	explicit Transform(std::string name = "Transform");
	~Transform() override;

	// Update
	// - position/scale/rotation 변경 시 월드 행렬과 right/up 벡터를 갱신
	// - 상수버퍼(WorldBuffer) 갱신은 dirty일 때만 수행
	void Update() override;

	// Render
	// - 월드 행렬 상수버퍼를 VS에 바인딩
	void Render() override;

	// GetRight / GetUp
	// - 현재 회전 기준의 로컬 축 벡터를 반환
	DirectX::SimpleMath::Vector2 GetRight() const { return right; }
	DirectX::SimpleMath::Vector2 GetUp() const { return up; }

	DirectX::SimpleMath::Vector2 GetPosition() const { return position; }
	DirectX::SimpleMath::Vector2 GetScale() const { return scale; }
	float GetRotationRadian() const { return rotation; }
	float GetRotationDegree() const;

	// SetPosition
	// - 위치를 설정하고, 값이 변경된 경우에만 dirty 처리
	void SetPosition(const DirectX::SimpleMath::Vector2& pos);

	// SetScale
	// - 스케일을 설정하고, 값이 변경된 경우에만 dirty 처리
	void SetScale(const DirectX::SimpleMath::Vector2& s);

	// SetRotationDegree
	// - 각도를 degree로 받아 radian으로 저장
	void SetRotationDegree(float degree);

	// SetRotationRadian
	// - 회전을 radian으로 설정
	void SetRotationRadian(float radian);

	// Move
	// - 위치를 누적 이동
	void Move(const DirectX::SimpleMath::Vector2& value);

	// AddScale
	// - 스케일을 누적 변경
	void AddScale(const DirectX::SimpleMath::Vector2& value);

	// RotateDegree / RotateRadian
	// - 회전을 누적 변경
	void RotateDegree(float degree);
	void RotateRadian(float radian);

private:
	DirectX::SimpleMath::Vector2 right = DirectX::SimpleMath::Vector2(1.0f, 0.0f);
	DirectX::SimpleMath::Vector2 up = DirectX::SimpleMath::Vector2(0.0f, 1.0f);

	DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
	DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2(1.0f, 1.0f);
	float rotation = 0.0f;

	DirectX::SimpleMath::Matrix world;
	std::unique_ptr<WorldBuffer> wb;

	bool bDirty = true;
};