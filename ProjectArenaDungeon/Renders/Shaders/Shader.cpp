#include "pch.h"
#include "Shader.h"

void Shader::CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob)
{
	ComPtr<ID3DBlob> error;
	
	HRESULT hr = D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		nullptr,
		entryName.c_str(),
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		blob,
		&error
	);
	CheckShaderError(hr, error);
}

void Shader::CheckShaderError(HRESULT hr, const ComPtr<ID3DBlob>& error)
{
	// shader error 발생 시 메시지 박스를 통해 에러 메시지 출력
	if (FAILED(hr))
	{
		if (error.Get())
		{
			const std::string& str = static_cast<const char*>(error->GetBufferPointer());
			// 메시지 박스 팝업 출력
			MessageBoxA(nullptr, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false && "Shader compilation failed");
	}
}
