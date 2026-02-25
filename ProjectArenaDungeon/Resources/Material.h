#pragma once

class ColorBuffer;
class FrameBuffer;
class InputLayout;
class VertexShader;
class PixelShader;
class Texture;

// Material
// - 렌더링에 필요한 ShaderSet(IL/VS/PS)과 Sampler, 텍스처, 상수 버퍼를 묶어 관리
// - MeshRenderer(Component)에서 Mesh와 함께 사용된다.
class Material
{
public:
	// Material
	// - shaderPath + layoutDesc로 ShaderSet(IL/VS/PS)을 가져오고,
	//   기본 Color/Frame 상수 버퍼를 초기화
	Material(std::wstring shaderPath, std::span<const D3D11_INPUT_ELEMENT_DESC> layoutDesc);

	// Bind
	// - InputLayout / VS / PS 바인딩
	// - 샘플러 상태 세팅
	// - Texture, ColorBuffer, FrameBuffer를 각 슬롯에 맞게 갱신/바인딩
	void Bind();

	void SetTexture(std::shared_ptr<Texture> texture) { this->texture = std::move(texture); }
	void SetTexture(const std::wstring& path);

	Color GetColor() const;
	void SetColor(Color color);

	// Point/Linear 샘플링 선택
	void SetPointSampler(bool val) { bPointSampling = val; }

	const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return frameBuffer; }

private:
	std::shared_ptr<InputLayout> il;
	std::shared_ptr<VertexShader> vs;
	std::shared_ptr<PixelShader> ps;

	std::shared_ptr<Texture> texture;

	std::unique_ptr<ColorBuffer> cb;
	std::shared_ptr<FrameBuffer> frameBuffer;

	bool bPointSampling = false;
};