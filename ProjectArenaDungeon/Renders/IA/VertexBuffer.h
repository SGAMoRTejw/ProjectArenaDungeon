#pragma once

#include <vector>

// VertexBuffer
// - 렌더링 파이프라인의 IA(Input Assembler) 단계에서 사용하는 정점 버퍼 클래스
// - 정점 데이터 생성(Create) 및 IA 바인딩(Bind)
class VertexBuffer
{
public:
	// Create
	// - CPU 메모리의 정점 데이터 -> GPU VertexBuffer로 생성
	// - IA 바인딩 목적에 대해 STAGING case 금지
	//		(desc flag 설정을 주석 처리 및 assert 추가)
	// - DYNAMIC 사용 시 CPUAccessFlags = WRITE 설정
	template <typename T>
	void Create(const std::vector<T>& vertices, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }

	// Bind
	// - IA 단계에 VertexBuffer 바인딩
	// - slot 0: vertex, slot 1: instancing 확장 대비
	void Bind(UINT slot = 0);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;
};

// 템플릿 함수 관련 링크 에러 방지
// 선언에 연속해서 함수 내용 작성, inline 키워드 사용(+.inl 파일)
template <typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, D3D11_USAGE usage)
{
	if (vertices.empty())
	{
		assert(false && "vertices vector is empty in VertexBuffer::Create()");
		return;
	}

	stride = sizeof(T);
	count = UINT(vertices.size());

	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = stride * count;
	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		break;
	case D3D11_USAGE_IMMUTABLE:
		desc.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		assert(false && "STAGING buffers cannot be bound to IA stage");
		//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		return;
	default:
		assert(false && "Unsupported D3D11_USAGE in VertexBuffer::Create()");
		return;
	}

	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = vertices.data();

	HRESULT hr = DEVICE->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);
}