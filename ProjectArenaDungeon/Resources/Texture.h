#pragma once

// Texture
// - 이미지 파일을 로드하고 ShaderResourceView(SRV)를 생성/관리하는 클래스
// - DirectXTex(LoadFromDDS/TGA/WIC, GenerateMipMaps)를 사용하여 다양한 포맷에 대응

class Texture
{
public:
	// Texture
	// - path의 확장자를 기반으로 로딩 루트를 선택(DDS/TGA/WIC)
	// - WIC 로드의 경우 mipmap을 생성하여 SRV를 생성
	explicit Texture(std::wstring path);

	// Bind
	// - PixelShader 단계에 SRV 바인딩 (기본 slot = 0)
	void Bind(UINT slot = 0);

	std::wstring GetPath() const { return path; }
	Vector2 GetSize() const { return Vector2((float)metaData.width, (float)metaData.height); }
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }

private:
	std::wstring path;
	DirectX::TexMetadata metaData{};
	DirectX::ScratchImage image{};
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
};