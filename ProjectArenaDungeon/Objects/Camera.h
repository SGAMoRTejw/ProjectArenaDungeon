#pragma once

#include "Objects/Object.h"

class ViewProjectionBuffer;

// Camera
// - 뷰/프로젝션 행렬을 계산하고 셰이더 상수 버퍼로 바인딩하는 오브젝트
// - 카메라 이동/줌 기능
class Camera final : public Object
{
public:
	Camera();

	void Update() override;

	// Bind
	// - 현재 view/projection을 VS 상수 버퍼 슬롯(b1)에 바인딩한다.
	void Bind() const;

	// 카메라 이동/줌
	void Move(DirectX::SimpleMath::Vector2 delta);
	void SetZoom(float value);
	float GetZoom() const;

	// 좌표 변환
	DirectX::SimpleMath::Vector2 ScreenToWorld(DirectX::SimpleMath::Vector2 screenPos) const;
	DirectX::SimpleMath::Vector2 WorldToScreen(DirectX::SimpleMath::Vector2 worldPos) const;

	DirectX::SimpleMath::Matrix GetView() const { return view; }
	DirectX::SimpleMath::Matrix GetProj() const { return proj; }

private:
	// RebuildMatrices
	// - Transform 값이 변했을 때 view 행렬을 갱신
	void RebuildMatrices();

private:
	DirectX::SimpleMath::Matrix view = {};
	DirectX::SimpleMath::Matrix proj = {};

	std::unique_ptr<ViewProjectionBuffer> vpBuffer;

	// NOTE:
	// - 값 변경 시에만 view 재계산을 하기 위한 캐시
	DirectX::SimpleMath::Vector2 lastPos = { -9999.0f, -9999.0f };
	float lastRot = -9999.0f;
	float lastZoom = -9999.0f;
};