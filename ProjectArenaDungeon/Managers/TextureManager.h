#pragma once

class Texture;

// TextureManager
// - 텍스처 로딩 및 캐싱(중복 생성 방지)을 담당하는 매니저
// - 동일 경로(path)의 텍스처 요청 시 기존 리소스를 재사용
class TextureManager
{
	DECLARE_SINGLETON(TextureManager)

public:
	// LoadTexture
	// - path 기반으로 Texture를 생성/캐싱하여 반환
	// - 이미 로드된 경우 캐시된 shared_ptr<Texture>를 반환
	std::shared_ptr<Texture> LoadTexture(const std::wstring& path);

private:
	// key: texture path
	// value: Texture 리소스(shared_ptr)
	std::unordered_map<std::wstring, std::shared_ptr<Texture>> cache;
};