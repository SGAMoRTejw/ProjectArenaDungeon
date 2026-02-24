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

class ShaderManager
{
	DECLARE_SINGLETON(ShaderManager)

public:
	ShaderSet GetShader(const std::wstring& path, std::span<const D3D11_INPUT_ELEMENT_DESC> descs);

private:
	std::unordered_map<std::wstring, ShaderSet> shaderCache;
};