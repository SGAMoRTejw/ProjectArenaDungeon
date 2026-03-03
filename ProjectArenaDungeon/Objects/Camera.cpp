#include "pch.h"
#include "Objects/Camera.h"

#include <cmath>

#include "Config/ConstantValues.h"
#include "Components/Transform.h"
#include "Renders/ConstantBuffers/GlobalBuffers.h"

Camera::Camera()
	: Object("MainCamera")
{
	vpBuffer = std::make_unique<ViewProjectionBuffer>();

	proj = DirectX::XMMatrixOrthographicOffCenterLH(0, gWinWidth, 0, gWinHeight, -1.0f, 1.0f);

	vpBuffer->SetProjectionMatrix(proj);

	// NOTE:
	// - 줌은 scale.x를 기준으로 사용(카메라는 균일 스케일만 허용)
	transform->SetScale({ 1.0f, 1.0f });
}

void Camera::Update()
{
	__super::Update();

	const DirectX::SimpleMath::Vector2 pos = transform->GetPosition();
	const float rot = transform->GetRotationRadian();
	const float zoom = transform->GetScale().x;

	// NOTE: float 비교는 epsilon 기반으로 처리한다.
	const bool samePos = (DirectX::SimpleMath::Vector2::DistanceSquared(pos, lastPos) <= epsilonSq);
	const bool sameRot = (std::fabs(rot - lastRot) <= epsilon);
	const bool sameZoom = (std::fabs(zoom - lastZoom) <= epsilon);

	if (samePos && sameRot && sameZoom)
		return;

	lastPos = pos;
	lastRot = rot;
	lastZoom = zoom;

	RebuildMatrices();

	vpBuffer->SetViewMatrix(view);
	vpBuffer->Update();
}

void Camera::RebuildMatrices()
{
	const DirectX::SimpleMath::Vector2 pos = transform->GetPosition();
	const float rot = transform->GetRotationRadian();
	const float zoom = transform->GetScale().x;

	// NOTE:
	// - 카메라는 월드의 반대 변환을 적용
	// - (카메라가 오른쪽으로 이동) == (월드가 왼쪽으로 이동)
	const DirectX::SimpleMath::Matrix T = DirectX::XMMatrixTranslation(-pos.x, -pos.y, 0.0f);
	const DirectX::SimpleMath::Matrix R = DirectX::XMMatrixRotationZ(-rot);
	const DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScaling(zoom, zoom, 1.0f);

	view = T * R * S;
}

void Camera::Bind() const
{
	// NOTE:
	// - HLSL: cbuffer ViewProjection : register(b1)
	vpBuffer->BindVS(1);
}

void Camera::Move(DirectX::SimpleMath::Vector2 delta)
{
	transform->Move(delta);
}

void Camera::SetZoom(float value)
{
	// NOTE: 0 이하 방지
	if (value < epsilon)
		value = epsilon;

	transform->SetScale({ value, value });
}

float Camera::GetZoom() const
{
	return transform->GetScale().x;
}

DirectX::SimpleMath::Vector2 Camera::ScreenToWorld(DirectX::SimpleMath::Vector2 screenPos) const
{
	// NOTE:
	// - 현재 버전은 "카메라 회전 미지원" 변환이다.
	// - 회전까지 포함하려면 view의 역행렬 기반 변환으로 확장한다.
	const float zoom = GetZoom();
	const DirectX::SimpleMath::Vector2 pos = transform->GetPosition();
	return (screenPos / zoom) + pos;
}

DirectX::SimpleMath::Vector2 Camera::WorldToScreen(DirectX::SimpleMath::Vector2 worldPos) const
{
	const float zoom = GetZoom();
	const DirectX::SimpleMath::Vector2 pos = transform->GetPosition();
	return (worldPos - pos) * zoom;
}