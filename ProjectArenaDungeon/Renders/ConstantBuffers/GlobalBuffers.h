#pragma once
#include "pch.h"
#include "ConstantBuffer.h"

// WorldData
// - 오브젝트의 월드 변환 행렬
struct WorldData
{
	Matrix world;
};

// WorldBuffer
// - 오브젝트 단위 월드 행렬을 전달하는 상수 버퍼
class WorldBuffer : public ConstantBuffer<WorldData>
{
public:
	WorldBuffer() : ConstantBuffer() {}

	void SetWorldMatrix(const Matrix& world)
	{
		data.world = world.Transpose();

		bDirty = true;
		// Transform에서 pos/scale/rot 변경이 감지된 시점에만 호출됨
		// 비용이 큰 행렬 비교는 사용하지 않음
	}
};

// ViewProjData
// - 카메라 View / Projection 행렬
struct ViewProjData
{
	Matrix view;
	Matrix proj;
};

// ViewProjectionBuffer
// - 카메라 시점(View)과 투영(Projection) 행렬을 전달하는 상수 버퍼
class ViewProjectionBuffer : public ConstantBuffer<ViewProjData>
{
public:
	ViewProjectionBuffer() : ConstantBuffer() {}

	void SetViewMatrix(const Matrix& view)
	{
		data.view = view.Transpose();

		bDirty = true;
	}

	void SetProjectionMatrix(const Matrix& proj)
	{
		data.proj = proj.Transpose();

		bDirty = true;
	}
};

// ColorData
// - 단색용 색상 정보
struct ColorData
{
	Color color;
};

// ColorBuffer
// - 단색에 사용되는 색상 상수 버퍼
class ColorBuffer : public ConstantBuffer<ColorData>
{
public:
	ColorBuffer() : ConstantBuffer() {}

	void SetColor(Color color)
	{
		if (data.color == color)
			return;

		data.color = color;
		bDirty = true;
	}

	Color GetColor() const { return data.color; }
};

// FrameData
// - 스프라이트/텍스처 아틀라스 프레임 정보 (UV 시작점, 크기)
struct FrameData
{
	Vector2 startUV;
	Vector2 sizeUV;
};

// FrameBuffer
// - 스프라이트 애니메이션 또는 아틀라스 프레임 정보를 셰이더로 전달
class FrameBuffer : public ConstantBuffer<FrameData>
{
public:
	FrameBuffer() : ConstantBuffer() {}

	void SetFrameData(Vector2 start, Vector2 size)
	{
		if (data.startUV == start && data.sizeUV == size)
			return;

		data.startUV = start;
		data.sizeUV = size;
		bDirty = true;
	}
};
