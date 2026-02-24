#pragma once
class InputLayout;
class VertexShader;
class PixelShader;

// ShaderSet
// - HLSL파일(path 기준)에서 사용하는 리소스 묶음
// - InputLayout, VertexShader, PixelShader를 함께 보관
struct ShaderSet
{
	std::shared_ptr<InputLayout> il;
	std::shared_ptr<VertexShader> vs;
	std::shared_ptr<PixelShader> ps;
};

// ShaderManager
// - 셰이더 로딩/생성 캐싱을 담당하는 매니저
// - 동일 path에 대해 재생성을 방지

class ShaderManager
{
	DECLARE_SINGLETON(ShaderManager)

public:
	// GetShader
	// - path와 입력 레이아웃을 기반으로 ShaderSet을 반환
	// - 캐시에 존재하면 해당 리소스를 반환, 없다면 생성 후 캐시에 저장
	ShaderSet GetShader(const std::wstring& path, std::span<const D3D11_INPUT_ELEMENT_DESC> descs);

private:
	// Shader cache
	// - key: shader file path
	// - value: ShaderSet
	std::unordered_map<std::wstring, ShaderSet> shaderCache;
};