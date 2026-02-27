#include "pch.h"
#include "Utilities/GeometryHelper.h"

#include <cmath>	// std::sinf, std::cosf
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Resources/Mesh.h"
#include "Resources/VertexType.h"

namespace
{
// NOTE:
// - 원 메시 생성은 최소 3 segments가 필요하다.
// - 0~2 값이 들어오면 원이 성립하지 않으므로 clamp 처리한다.
constexpr UINT MIN_CIRCLE_SEGMENTS = 3;

UINT ClampSegments(UINT segments)
{
	return std::max(segments, MIN_CIRCLE_SEGMENTS);
}
}

namespace GeometryHelper
{
std::shared_ptr<Mesh> CreateRectangle()
{
	static std::shared_ptr<Mesh> mesh;
	if (mesh) return mesh;

	std::vector<Vertex> vertices(4);
	vertices[0].position = { -0.5f, -0.5f };
	vertices[1].position = { -0.5f,  0.5f };
	vertices[2].position = { 0.5f, -0.5f };
	vertices[3].position = { 0.5f,  0.5f };

	std::vector<UINT> indices = { 0, 1, 2, 2, 1, 3 };

	mesh = std::make_shared<Mesh>();
	mesh->Create(vertices, indices);
	return mesh;
}

std::shared_ptr<Mesh> CreateLine()
{
	static std::shared_ptr<Mesh> mesh;
	if (mesh) return mesh;

	std::vector<Vertex> vertices(2);
	vertices[0].position = { 0.0f, 0.0f };
	vertices[1].position = { 1.0f, 0.0f };

	std::vector<UINT> indices = { 0, 1 };

	mesh = std::make_shared<Mesh>();
	mesh->Create(vertices, indices);
	return mesh;
}

std::shared_ptr<Mesh> CreateWireCircle(UINT segments)
{
	segments = ClampSegments(segments);

	static std::unordered_map<UINT, std::shared_ptr<Mesh>> cache;
	if (auto it = cache.find(segments); it != cache.end())
		return it->second;

	std::vector<Vertex> vertices(segments);
	for (UINT i = 0; i < segments; ++i)
	{
		const float theta = 2.0f * DirectX::XM_PI * static_cast<float>(i) / static_cast<float>(segments);
		// NOTE:
		// - 반지름 0.5 기준의 단위 원을 만든다.
		// - 실제 크기는 Transform의 scale로 조절한다.
		vertices[i].position = DirectX::SimpleMath::Vector2(std::sinf(theta), std::cosf(theta)) * 0.5f;
	}

	// NOTE:
	// - LINESTRIP로 닫힌 원을 만들기 위해 마지막에 0번 정점을 다시 넣는다.
	std::vector<UINT> indices;
	indices.reserve(segments + 1);
	for (UINT i = 0; i < segments; ++i)
		indices.push_back(i);
	indices.push_back(0);

	auto mesh = std::make_shared<Mesh>();
	mesh->Create(vertices, indices);

	cache.emplace(segments, mesh);
	return mesh;
}

std::shared_ptr<Mesh> CreateSolidCircle(UINT segments)
{
	segments = ClampSegments(segments);

	static std::unordered_map<UINT, std::shared_ptr<Mesh>> cache;
	if (auto it = cache.find(segments); it != cache.end())
		return it->second;

	// NOTE:
	// - 0번은 중심점
	// - 1 ~ segments는 외곽 점
	std::vector<Vertex> vertices(segments + 1);
	vertices[0].position = { 0.0f, 0.0f };

	for (UINT i = 1; i <= segments; ++i)
	{
		const float theta = 2.0f * DirectX::XM_PI * static_cast<float>(i - 1) / static_cast<float>(segments);
		vertices[i].position = DirectX::SimpleMath::Vector2(std::sinf(theta), std::cosf(theta)) * 0.5f;
	}

	// NOTE:
	// - 삼각형을 이어붙이는 형태(중점-외곽-외곽)로 인덱스 구성
	std::vector<UINT> indices;
	indices.reserve(segments * 3);

	for (UINT i = 0; i < segments; ++i)
	{
		const UINT a = 0;
		const UINT b = i + 1;
		const UINT c = (i + 2 > segments) ? 1 : (i + 2);

		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	auto mesh = std::make_shared<Mesh>();
	mesh->Create(vertices, indices);

	cache.emplace(segments, mesh);
	return mesh;
}

std::shared_ptr<Mesh> CreateTexturedQuad()
{
	static std::shared_ptr<Mesh> mesh;
	if (mesh) return mesh;

	std::vector<VertexTexture> vertices(4);
	vertices[0].position = { -0.5f, -0.5f };
	vertices[1].position = { -0.5f,  0.5f };
	vertices[2].position = { 0.5f, -0.5f };
	vertices[3].position = { 0.5f,  0.5f };

	// NOTE:
	// - 텍스처 좌표(UV)는 (좌상단 0,0) 기준으로 매핑
	vertices[0].uv = { 0.0f, 1.0f };
	vertices[1].uv = { 0.0f, 0.0f };
	vertices[2].uv = { 1.0f, 1.0f };
	vertices[3].uv = { 1.0f, 0.0f };

	std::vector<UINT> indices = { 0, 1, 2, 2, 1, 3 };

	mesh = std::make_shared<Mesh>();
	mesh->Create(vertices, indices);
	return mesh;
}
}