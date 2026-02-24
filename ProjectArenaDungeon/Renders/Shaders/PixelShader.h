#pragma once
#include "pch.h"
#include "Shader.h"

// PixelShader
// - HLSL Pixel Shader를 컴파일하고 PixelShader 리소스를 생성하는 클래스
class PixelShader : public Shader
{
public:
	// Create
	// - HLSL 파일 경로와 entryName을 기준으로 컴파일하고 리소스를 생성
	void Create(const std::wstring& path, const std::string& entryName) override;

	// Clear
	// - 리소스 및 컴파일 결과(blob)를 해제
	void Clear() override;

	// Bind
	// - 렌더링 파이프라인(PS 단계)에 PixelShader를 바인딩
	void Bind() override;

	// GetResource
	// - 생성된 ID3D11PixelShader 리소스를 반환
	ID3D11PixelShader* GetResource() const { return shader.Get(); }

private:
	ComPtr<ID3DBlob> blob;				// 컴파일된 바이트코드
	ComPtr<ID3D11PixelShader> shader;	// PixelShader 리소스
};