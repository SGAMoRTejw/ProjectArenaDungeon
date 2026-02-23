#pragma once
#include "pch.h"

template <typename T>
class ConstantBuffer
{
public:
	virtual ~ConstantBuffer() = default;

	void Update()
	{
		// 데이터 변경 내용이 없다면 더 이상 진행하지 않음
		if (!bDirty) return;

		// map - unmap을 통한 데이터 변경
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		HRESULT hr = DEVICE_CONTEXT->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		CHECK(hr);
		{
			memcpy(mappedSubResource.pData, &data, sizeof(T));
		}
		DEVICE_CONTEXT->Unmap(buffer.Get(), 0);

		bDirty = false;
	}

	// 렌더링 파이프라인에 바인딩
	void BindVS(UINT slot)
	{
		DEVICE_CONTEXT->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

	void BindPS(UINT slot)
	{
		DEVICE_CONTEXT->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

protected:
	ConstantBuffer()
	{
		// buffer description
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = (sizeof(T) + 15) & ~15;			// 16byte padding 적용
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;

		// buffer 생성
		HRESULT hr = DEVICE->CreateBuffer(&desc, nullptr, &buffer);
		CHECK(hr);
	}

protected:
	T data;
	bool bDirty = true;				// 데이터 변경 확인용 flag

private:
	ComPtr<ID3D11Buffer> buffer;
};