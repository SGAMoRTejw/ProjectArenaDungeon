#pragma once
#include "pch.h"
#include "Shader.h"

// VertexShader
// - HLSL Vertex Shader를 컴파일하고 Vertex Shader 리소스를 생성하는 클래스
// - InputLayout 생성 시 필요한 바이트코드(blob)를 보관
class VertexShader : public Shader
{
public:
	// Create
	// - HLSL 파일 경로와 entryName을 기준으로 컴파일하고 리소스를 생성
	void Create(const std::wstring& path, const std::string& entryName) override;

	// Clear
	// - 리소스 및 컴파일 결과(blob)를 해제
	void Clear() override;

	// Bind
	// - 렌더링 파이프라인(VS 단계)에 VertexShader를 바인딩
	void Bind() override;

	// GetBlob
	// - 컴파일된 바이트코드를 반환
	ID3DBlob* GetBlob() const { return blob.Get(); }

	// GetResource
	// - 생성된 ID3D11VertexShader 리소스를 반환
	ID3D11VertexShader* GetResource() const { return shader.Get(); }

private:
	ComPtr<ID3DBlob> blob;				// 컴파일된 바이트코드
	ComPtr<ID3D11VertexShader> shader;	// VertexShader 리소스
};