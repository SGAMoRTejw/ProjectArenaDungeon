#include "pch.h"
#include "InputLayout.h"

void InputLayout::Create(std::span<const D3D11_INPUT_ELEMENT_DESC> descs, const ComPtr<ID3DBlob>& blob)
{
	if (descs.empty() || !blob.Get())
	{
		assert(false && "Invalid arguments in InputLayout::Create()");
		return;
	}

	HRESULT hr = DEVICE->CreateInputLayout(
		descs.data(),
		UINT(descs.size()),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout
	);
	CHECK(hr);
}

void InputLayout::Bind()
{
	DEVICE_CONTEXT->IASetInputLayout(inputLayout.Get());
}
