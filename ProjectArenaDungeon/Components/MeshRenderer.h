#pragma once

#include <memory>
#include <string>

// D3D11_PRIMITIVE_TOPOLOGY
#include <d3d11.h>

#include "Components/Component.h"

class Mesh;
class Material;

// MeshRenderer
// - Mesh + Material을 결합하여 렌더링을 수행하는 컴포넌트
// - 현재는 MeshRenderer가 직접 DrawCall을 수행
// - NOTE: 이후 Instancing 도입 시 RenderSystem으로 DrawCall 요청 구조로 이관 예정
class MeshRenderer final : public Component
{
public:
	explicit MeshRenderer(std::string name = "MeshRenderer");
	~MeshRenderer() override = default;

	// Render
	// - Mesh/Material이 준비된 경우에만 파이프라인에 바인딩 후 DrawIndexed 수행
	void Render() override;

	void SetMesh(std::shared_ptr<Mesh> value) { mesh = std::move(value); }
	void SetMaterial(std::shared_ptr<Material> value) { material = std::move(value); }

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY value) { topology = value; }

	const std::shared_ptr<Material>& GetMaterial() const { return material; }

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};