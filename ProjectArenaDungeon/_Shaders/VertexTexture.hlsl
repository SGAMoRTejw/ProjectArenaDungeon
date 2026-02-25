#pragma pack_matrix(row_major)

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer World : register(b0)
{
    matrix _world;
}

cbuffer ViewPorjection : register(b1)
{
    matrix _view;
    matrix _proj;
}

cbuffer FrameBuffer : register(b10)
{
    float2 _startUV;
    float2 _sizeUV;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    // Transpose 정책에 따라 mul 순서 바뀔 수 있음
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _proj);

    // Atlas frame
    output.uv = _startUV + (input.uv * _sizeUV);

    return output;
}

cbuffer Color : register(b2)
{
    float4 _color;
}

SamplerState samp : register(s0);   // 0~16
Texture2D srcTex : register(t0);    // 0~128

float4 PS(PixelInput input) : SV_Target0
{
    float4 texColor = srcTex.Sample(samp, input.uv);
    
    // MAGENTA 컬러 키 처리
    if (texColor.r == 1.0f && texColor.g == 0.0f && texColor.b == 1.0f)
        discard;
    
    return texColor * _color;
}