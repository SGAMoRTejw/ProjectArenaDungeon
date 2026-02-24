#include "pch.h"
#include "IndexBuffer.h"

void IndexBuffer::Create(const std::vector<UINT>& indices, D3D11_USAGE usage)
{
	if (indices.empty())
	{
		assert(false && "indices vector is empty in IndexBuffer::Create()");
		return;
	}

	stride = sizeof(UINT);
	count = UINT(indices.size());

	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = stride * count;
	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

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
		assert(false && "Unsupported D3D11_USAGE in IndexBuffer::Create()");
		return;
	}

	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = indices.data();

	HRESULT hr = DEVICE->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);
}

void IndexBuffer::Bind()
{
	DEVICE_CONTEXT->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}