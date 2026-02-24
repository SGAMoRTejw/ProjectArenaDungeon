#include "pch.h"
#include "PixelShader.h"

void PixelShader::Create(const std::wstring& path, const std::string& entryName)
{
	this->path = path;
	this->entryName = entryName;

	// HLSL 컴파일 (ps_5_0)
	CompileShader(this->path, this->entryName, "ps_5_0", &blob);

	// 리소스 생성
	HRESULT hr = DEVICE->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	CHECK(hr);
}

void PixelShader::Clear()
{
	// 리소스 해제
	shader.Reset();
	blob.Reset();
}

void PixelShader::Bind()
{
	// PS 단계에 바인딩
	DEVICE_CONTEXT->PSSetShader(shader.Get(), nullptr, 0);
}
