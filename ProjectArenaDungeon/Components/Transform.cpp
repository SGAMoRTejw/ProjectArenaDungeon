#include "pch.h"
#include "Components/Transform.h"
#include "Config/ConstantValues.h"

#include "Renders/ConstantBuffers/GlobalBuffers.h"

using Vector2 = DirectX::SimpleMath::Vector2;
using Matrix = DirectX::SimpleMath::Matrix;

constexpr float epsilonSq = epsilon * epsilon;

Transform::Transform(std::string name)
	: Component(std::move(name))
{
	wb = std::make_unique<WorldBuffer>();
}

Transform::~Transform() = default;

float Transform::GetRotationDegree() const
{
	return DirectX::XMConvertToDegrees(rotation);
}

void Transform::SetPosition(const Vector2& pos)
{
	if (position != pos)
	{
		position = pos;
		bDirty = true;
	}
}

void Transform::SetScale(const Vector2& s)
{
	if (scale != s)
	{
		scale = s;
		bDirty = true;
	}
}

void Transform::SetRotationDegree(float degree)
{
	const float radian = DirectX::XMConvertToRadians(degree);
	SetRotationRadian(radian);
}

void Transform::SetRotationRadian(float radian)
{
	if (rotation != radian)
	{
		rotation = radian;
		bDirty = true;
	}
}

void Transform::Move(const Vector2& value)
{
	if (value.LengthSquared() < epsilonSq)
		return;

	position += value;
	bDirty = true;
}

void Transform::AddScale(const Vector2& value)
{
	if (value.LengthSquared() < epsilonSq)
		return;

	scale += value;
	bDirty = true;
}

void Transform::RotateDegree(float degree)
{
	if (std::abs(degree) < epsilon)
		return;

	rotation += DirectX::XMConvertToRadians(degree);
	bDirty = true;
}

void Transform::RotateRadian(float radian)
{
	if (std::abs(radian) < epsilon)
		return;

	rotation += radian;
	bDirty = true;
}

void Transform::Update()
{
	if (!bDirty)
		return;

	// NOTE: 화면 좌표계/회전 방향 정책에 맞춰 Z 회전 부호 변경
	Matrix S = DirectX::XMMatrixScalingFromVector(scale);
	Matrix R = DirectX::XMMatrixRotationZ(-rotation);
	Matrix T = DirectX::XMMatrixTranslationFromVector(position);

	// NOTE: 회전이 갱신되는 시점에만 로컬 축 벡터를 캐싱
	float sinV = 0.0f;
	float cosV = 0.0f;
	DirectX::XMScalarSinCos(&sinV, &cosV, -rotation);

	right = Vector2(cosV, sinV);
	up = Vector2(-sinV, cosV);

	world = S * R * T;

	wb->SetWorldMatrix(world);
	wb->Update();

	bDirty = false;
}

void Transform::Render()
{
	// NOTE: 월드 버퍼는 기본 VS 슬롯 0을 사용
	wb->BindVS(0);
}