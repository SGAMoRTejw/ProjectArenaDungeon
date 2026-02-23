#pragma once
#include "pch.h"
#include "ConstantBuffer.h"

struct WorldData
{
	Matrix world;
};

class WorldBuffer : public ConstantBuffer<WorldData>
{
public:
	WorldBuffer() : ConstantBuffer() {}

	void SetWorldMatrix(const Matrix& world)
	{
		data.world = world.Transpose();

		bDirty = true;
		// Transform에서 pos, scale, rot에 대해서 체크
		// 데이터 변경이 확실한 시점 이므로 무거운 행렬 비교를 사용하지 않음
	}
};

struct ViewProjData
{
	Matrix view;
	Matrix proj;
};

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

struct ColorData
{
	Color color;
};

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

struct FrameData
{
	Vector2 startUV;
	Vector2 sizeUV;
};

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
