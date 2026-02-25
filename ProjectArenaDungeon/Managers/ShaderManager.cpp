#include "pch.h"
#include "ShaderManager.h"

#include "Renders/IA/InputLayout.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"

ShaderManager::ShaderManager() {}

ShaderSet ShaderManager::GetShader(const std::wstring& path, std::span<const D3D11_INPUT_ELEMENT_DESC> descs)
{
	// path를 기준으로 캐시 조회
	auto it = shaderCache.find(path);

	if (it != shaderCache.end())
		return it->second;

	// 새 ShaderSet 생성
	ShaderSet newSet;

	// VertexShader 생성
	newSet.vs = std::make_shared<VertexShader>();
	newSet.vs->Create(path, "VS");

	// InputLayout 생성
	newSet.il = std::make_shared<InputLayout>();
	newSet.il->Create(descs, newSet.vs->GetBlob());

	// PixelShader 생성
	newSet.ps = std::make_shared<PixelShader>();
	newSet.ps->Create(path, "PS");

	// 캐시에 저장
	shaderCache.emplace(path, newSet);

	return newSet;
}
