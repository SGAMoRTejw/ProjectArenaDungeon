#include "pch.h"
#include "ObjectFactory.h"

#include <span>

#include "Objects/Object.h"

#include "Resources/Material.h"
#include "Resources/Texture.h"
#include "Resources/VertexType.h"
#include "Resources/Mesh.h"

#include "Components/MeshRenderer.h"
// NOTE: make_shared<MeshRenderer>()에 필요

#include "Utilities/GeometryHelper.h"

namespace ObjectFactory
{
namespace
{
// AttachMeshAndMaterial
// - Object에 MeshRenderer를 생성/부착하고, Mesh/Material/Topology를 세팅
//
// NOTE:
// - shaderPath + input layout descs는 Material 생성에 사용된다.
// - texturePath가 비어있지 않으면 Texture를 로드하여 Material에 연결한다.
void AttachMeshAndMaterial(
	const std::shared_ptr<Object>& obj,
	const std::shared_ptr<Mesh>& mesh,
	D3D11_PRIMITIVE_TOPOLOGY topology,
	Color color,
	const std::wstring& shaderPath,
	std::span<const D3D11_INPUT_ELEMENT_DESC> descs,
	const std::wstring& texturePath = L""
)
{
	auto renderer = make_shared<MeshRenderer>();

	renderer->SetMesh(mesh);
	renderer->SetTopology(topology);

	auto mat = make_shared<Material>(shaderPath, descs);
	mat->SetColor(color);

	if (texturePath.empty() == false)
	{
		auto tex = TEXTURES.LoadTexture(texturePath);
		mat->SetTexture(tex);
	}

	renderer->SetMaterial(move(mat));

	obj->AddComponent(renderer);
}
}

std::shared_ptr<Object> CreateSprite(Vector2 position, Vector2 scale, float rotation, const std::wstring& path)
{
	auto obj = std::make_shared<Object>("Sprite", position, scale, rotation);

	AttachMeshAndMaterial(
		obj,
		GeometryHelper::CreateTexturedQuad(),
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		Color(1, 1, 1, 1),
		L"_Shaders/VertexTexture.hlsl",
		VertexTexture::descs,
		path
	);

	return obj;
}

std::shared_ptr<Object> ObjectFactory::CreateColorRect(Vector2 position, Vector2 scale, float rotation, Color color)
{
	auto obj = std::make_shared<Object>("ColorRect", position, scale, rotation);

	AttachMeshAndMaterial(obj, GeometryHelper::CreateRectangle(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, color, L"_Shaders/Vertex.hlsl", Vertex::descs);

	return obj;
}

std::shared_ptr<Object> ObjectFactory::CreateLine(Vector2 position, float length, float rotation, Color color)
{
	auto obj = std::make_shared<Object>("Line", position, Vector2(length, 1.0f), rotation);

	AttachMeshAndMaterial(obj, GeometryHelper::CreateLine(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST, color, L"_Shaders/Vertex.hlsl", Vertex::descs);

	return obj;
}

std::shared_ptr<Object> ObjectFactory::CreateWireCircle(Vector2 position, Vector2 scale, float rotation, Color color, UINT segments)
{
	auto obj = std::make_shared<Object>("WireCircle", position, scale, rotation);

	AttachMeshAndMaterial(obj, GeometryHelper::CreateWireCircle(segments), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, color, L"_Shaders/Vertex.hlsl", Vertex::descs);

	return obj;
}

std::shared_ptr<Object> ObjectFactory::CreateSolidCircle(Vector2 position, Vector2 scale, float rotation, Color color, UINT segments)
{
	auto obj = std::make_shared<Object>("SolidCircle", position, scale, rotation);

	AttachMeshAndMaterial(obj, GeometryHelper::CreateSolidCircle(segments), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, color, L"_Shaders/Vertex.hlsl", Vertex::descs);

	return obj;
}
}