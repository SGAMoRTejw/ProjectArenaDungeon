#include "pch.h"
#include "TextureManager.h"
#include "Resources/Texture.h"

TextureManager::TextureManager() {}

std::shared_ptr<Texture> TextureManager::LoadTexture(const std::wstring& path)
{
	// 이미 로드된 텍스처면 캐시 반환
	auto it = cache.find(path);
	if (it != cache.end())
		return it->second;

	// 새 텍스처 생성 후 캐시에 저장
	auto tex = std::make_shared<Texture>(path);
	cache.emplace(path, tex);

	return tex;
}