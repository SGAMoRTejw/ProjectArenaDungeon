#pragma once
// InputLayout
// - 렌더링 파이프라인 IA(Input Assembler) 단계에서 사용하는 입력 레이아웃 래퍼
// - VertexShader의 입력(Blob)과 정점 레이아웃(D3D11_INPUT_ELEMENT_DESC)을 기반으로 생성(Create)
// - IA 단계 바인딩(Bind)

class InputLayout
{
public:
	// Create
	// - 정점 레이아웃(desc)과 VertexShader 바이트코드(blob)을 이용해 InputLayout 생성
	void Create(std::span<const D3D11_INPUT_ELEMENT_DESC> descs, const Microsoft::WRL::ComPtr<ID3DBlob>& blob);

	// Bind
	// - IA 단계에 InputLayout 바인딩
	void Bind();

	ID3D11InputLayout* GetResource() const { return inputLayout.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};