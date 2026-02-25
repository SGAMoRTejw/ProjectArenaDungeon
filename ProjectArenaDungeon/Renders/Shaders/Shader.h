#pragma once

// Shader
// - 셰이더 기반 클래스
// - 파생 클래스(VertexShader / PixelShader)에서 실제 리소스 생성(Create) 및 바인딩(Bind)을 구현
// - CompileShader()를 통해 HLSL을 컴파일
class Shader
{
public:

	// Create
	// - path의 HLSL 파일을 entryName기준으로 컴파일하고,
	//   파생 클래스에서 리소스(VertexShader, PixelShader 등)를 생성
	virtual void Create(const std::wstring& path, const std::string& entryName) = 0;

	// Clear
	// - 보유한 리소스를 해제
	virtual void Clear() = 0;

	// Bind
	// - 렌더링 파이프라인에 셰이더를 바인딩
	virtual void Bind() = 0;

protected:
	// CompileShader
	// - HLSL 파일(path)을 컴파일하여 blob 생성
	void CompileShader(const std::wstring& path, const std::string& entryName, const std::string& profile, ID3DBlob** blob);

private:
	// CheckShaderError
	// - 컴파일 실패 시 메시지 박스를 통해 에러 메시지를 출력
	void CheckShaderError(HRESULT hr, const ComPtr<ID3DBlob>& error);

protected:
	std::wstring path = L"";
	std::string entryName = "";
};