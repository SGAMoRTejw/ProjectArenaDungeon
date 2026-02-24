#include "pch.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager() {}

ShaderSet ShaderManager::GetShader(const std::wstring& path, std::span<const D3D11_INPUT_ELEMENT_DESC> descs)
{
	auto it = shaderCache.find(path);

	if (it != shaderCache.end())
		return it->second;

	ShaderSet newSet;

	newSet.vs = std::make_shared<VertexShader>();
	newSet.vs->Create(path, "VS");

	newSet.il = std::make_shared<InputLayout>();
	newSet.il->Create(descs, newSet.vs->GetBlob());

	newSet.ps = std::make_shared<PixelShader>();
	newSet.ps->Create(path, "PS");

	shaderCache.emplace(path, newSet);

	return newSet;
}
