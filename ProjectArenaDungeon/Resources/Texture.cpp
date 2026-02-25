#include "pch.h"
#include "Texture.h"

using namespace DirectX;

Texture::Texture(std::wstring path)
	: path(std::move(path))
{
	// 확장자 추출 (모두 대문자로 처리)
	std::wstring ext = this->path.substr(this->path.find_last_of(L".") + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), ::towupper);

	HRESULT hr = S_OK;

	if (ext == L"DDS")
	{
		hr = LoadFromDDSFile(this->path.c_str(), DDS_FLAGS_NONE, &metaData, image);
		CHECK(hr);
	}
	else if (ext == L"TGA")
	{
		hr = LoadFromTGAFile(this->path.c_str(), &metaData, image);
		CHECK(hr);
	}
	else
	{
		ScratchImage tempImage;

		hr = LoadFromWICFile(this->path.c_str(), WIC_FLAGS_NONE, &metaData, tempImage);
		CHECK(hr);

		hr = GenerateMipMaps(
			tempImage.GetImages(),
			tempImage.GetImageCount(),
			tempImage.GetMetadata(),
			TEX_FILTER_DEFAULT,
			0,
			image
		);
		CHECK(hr);

		metaData = image.GetMetadata();
	}

	// SRV 생성
	// NOTE: 매크로 DEVICE는 ID3D11Device*를 반환
	hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), metaData, &srv);
	CHECK(hr);
}

void Texture::Bind(UINT slot)
{
	// PS 단계에 SRV 바인딩
	DEVICE_CONTEXT->PSSetShaderResources(slot, 1, srv.GetAddressOf());
}