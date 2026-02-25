#pragma once

// VertexType
// - InputLayout 생성에 필요한 입력 description과 정점 데이터 구조를 정의

using namespace DirectX::SimpleMath;

struct Vertex
{
	Vertex() = default;
	explicit Vertex(Vector2 position)
		: position(position)
	{
	}

	Vector2 position;

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColor
{
	VertexColor() = default;
	VertexColor(Vector2 position, Color color)
		: position(position), color(color)
	{
	}

	Vector2 position;
	Color color;

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexTexture
{
	VertexTexture() = default;
	VertexTexture(Vector2 position, Vector2 uv)
		: position(position), uv(uv)
	{
	}

	Vector2 position;
	Vector2 uv;

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};