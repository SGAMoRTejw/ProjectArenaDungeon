#include "pch.h"
#include "Components/MeshRenderer.h"

#include "Resources/Mesh.h"
#include "Resources/Material.h"

MeshRenderer::MeshRenderer(std::string name)
	: Component(std::move(name))
{
}

void MeshRenderer::Render()
{
	// NOTE:
	// - Mesh/Material이 없는 경우 렌더링을 수행하지 않는다.
	if (!mesh || !material)
		return;

	const UINT indexCount = mesh->GetIndexCount();
	if (indexCount == 0)
		return;

	mesh->Bind();
	material->Bind();

	DEVICE_CONTEXT->IASetPrimitiveTopology(topology);
	DEVICE_CONTEXT->DrawIndexed(indexCount, 0, 0);
}