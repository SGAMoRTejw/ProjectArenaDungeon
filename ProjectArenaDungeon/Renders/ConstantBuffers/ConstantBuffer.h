#pragma once
#include "pch.h"

// ConstantBuffer<T>
// - 상수 버퍼의 공용 템플릿 클래스
// - CPU -> GPU 상수 데이터 전송 및 셰이더 바인딩
// - D3D11_USAGE_DYNAMIC + Map/Unmap 방식으로 갱신
// - 16byte 정렬 규칙을 만족하도록 설계

template <typename T>
class ConstantBuffer
{
public:
	virtual ~ConstantBuffer() = default;

	// Update
	// - bDirty 플래그를 이용해 변경이 있을 때만 실제 Map/Unmap 수행
	// - D3D11_MAP_WRITE_DISCARD 방식으로 전체 덮어쓰기
	void Update()
	{
		// 데이터 변경 내용이 없다면 더 이상 진행하지 않음
		if (!bDirty) return;

		// Map/Unmap을 통한 데이터 갱신
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		HRESULT hr = DEVICE_CONTEXT->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		CHECK(hr);
		{
			memcpy(mappedSubResource.pData, &data, sizeof(T));
		}
		DEVICE_CONTEXT->Unmap(buffer.Get(), 0);

		bDirty = false;
	}

	// BindVS
	// - VertexShader 단계에 ConstantBuffer 바인딩
	void BindVS(UINT slot)
	{
		DEVICE_CONTEXT->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

	// BindPS
	// - PixelShader 단계에 ConstantBuffer 바인딩
	void BindPS(UINT slot)
	{
		DEVICE_CONTEXT->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

protected:
	// ConstantBuffer
	// - D3D11_USAGE_DYNAMIC 상수 버퍼 생성
	// - 16byte 정렬 규칙을 따르므로 ByteWidth를 16의 배수로 패딩
	ConstantBuffer()
	{
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = (sizeof(T) + 15) & ~15;			// 16byte padding 적용
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;

		HRESULT hr = DEVICE->CreateBuffer(&desc, nullptr, &buffer);
		CHECK(hr);
	}

protected:
	T data;							// CPU 데이터
	bool bDirty = true;				// 데이터 변경 여부 플래그 (불필요한 Map/Unmap 방지)

private:
	ComPtr<ID3D11Buffer> buffer;	// GPU 상수 버퍼 리소스
};