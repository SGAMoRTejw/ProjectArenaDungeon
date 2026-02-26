#pragma once

#include <vector>

// IndexBuffer
// - 렌더링 파이프라인의 IA(Input Assembler) 단계에서 사용하는 인덱스 버퍼 클래스
// - 인덱스 데이터 생성(Create) 및 IA 바인딩(Bind)
class IndexBuffer
{
public:
	// Create
	// - CPU 메모리의 인덱스 데이터 -> GPU IndexBuffer로 생성
	// - IA 바인딩 목적에 대해 STAGING case 금지
	//		(desc flag 설정을 주석 처리 및 assert 추가)
	// - DYNAMIC 사용 시 CPUAccessFlags = WRITE 설정
	void Create(const std::vector<UINT>& indices, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }

	// Bind
	// - IA 단계에 IndexBuffer 바인딩
	void Bind();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;
};