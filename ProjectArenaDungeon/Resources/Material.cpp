#include "pch.h"
#include "Material.h"

#include "Renders/IA/InputLayout.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "Renders/ConstantBuffers/GlobalBuffers.h"

#include "Resources/Texture.h"

Material::Material(std::wstring shaderPath, std::span<const D3D11_INPUT_ELEMENT_DESC> layoutDesc)
{
	// ShaderSet 가져오기 (IL/VS/PS)
	ShaderSet shaders = SHADERS.GetShader(shaderPath, layoutDesc);
	il = shaders.il;
	vs = shaders.vs;
	ps = shaders.ps;

	// 기본 색상(계수) = 흰색
	cb = std::make_unique<ColorBuffer>();
	cb->SetColor(Color(1, 1, 1, 1));

	// 기본 프레임(전체 UV)
	frameBuffer = std::make_shared<FrameBuffer>();
	frameBuffer->SetFrameData(Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
}

void Material::SetTexture(const std::wstring& path)
{
	texture = TEXTURES.LoadTexture(path);
}

Color Material::GetColor() const
{
	return cb ? cb->GetColor() : Color(1, 1, 1, 1);
}

void Material::SetColor(Color color)
{
	if (cb) cb->SetColor(color);
}

void Material::Bind()
{
	// IA/Shader 바인딩
	if (il) il->Bind();
	if (vs) vs->Bind();
	if (ps) ps->Bind();

	// 샘플러 상태
	GRAPHICS.SetSampler(bPointSampling);

	// 텍스처 바인딩 (기본 slot 0)
	if (texture) texture->Bind(0);

	// ColorBuffer 바인딩
	// NOTE: HLSL의 Color cbuffer register(b2)
	if (cb)
	{
		cb->Update();
		cb->BindPS(2);
	}

	// FrameBuffer 바인딩
	// NOTE: HLSL의 FrameBuffer register(b10)
	if (frameBuffer)
	{
		frameBuffer->Update();
		frameBuffer->BindVS(10);
	}
}