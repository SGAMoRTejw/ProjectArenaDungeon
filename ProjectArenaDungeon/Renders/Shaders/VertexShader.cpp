#include "pch.h"
#include "VertexShader.h"

void VertexShader::Create(const std::wstring& path, const std::string& entryName)
{
	this->path = path;
	this->entryName = entryName;

	// HLSL 컴파일 (vs_5_0)
	CompileShader(this->path, this->entryName, "vs_5_0", &blob);
	
	// 리소스 생성
	HRESULT hr = DEVICE->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	CHECK(hr);
}

void VertexShader::Clear()
{
	// 리소스 해제
	shader.Reset();
	blob.Reset();
}

void VertexShader::Bind()
{
	// VS 단계에 바인딩
	DEVICE_CONTEXT->VSSetShader(shader.Get(), nullptr, 0);
}
