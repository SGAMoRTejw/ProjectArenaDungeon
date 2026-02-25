#include "pch.h"
#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
    Initialize();
}

void GraphicsManager::Initialize()
{
    // SwapChain description
    DXGI_SWAP_CHAIN_DESC desc{};

    desc.BufferDesc.Width = 0;
    desc.BufferDesc.Height = 0;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 2;
    desc.OutputWindow = gHandle;
    desc.Windowed = true;
    // Tearing 허용
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    UINT flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    //Blue Green Red Alpha 색상 지원

#ifdef _DEBUG
    flag |= D3D11_CREATE_DEVICE_DEBUG;
    // VS에서 에러 내용 확인을 위한 flag
#endif

    // 지원 feature level 우선순위
    std::vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    // Device + SwapChain 생성
    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flag,
        featureLevels.data(),
        UINT(featureLevels.size()),
        D3D11_SDK_VERSION,
        &desc,
        &swapChain,
        &device,
        //&currentFeature,
        nullptr,
        &deviceContext
    );
    CHECK(hr);

    // Sampler state 생성 (Linear / Point)
    CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    hr = device->CreateSamplerState(&samplerDesc, &samplerLinear);
    CHECK(hr);

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    hr = device->CreateSamplerState(&samplerDesc, &samplerPoint);
    CHECK(hr);

    deviceContext->PSSetSamplers(0, 1, samplerLinear.GetAddressOf());

    // Alpha blending state 생성
    CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = device->CreateBlendState(&blendDesc, &blendState);
    CHECK(hr);

    // BackBuffer RTV + viewport
    CreateBackBuffer();
}

void GraphicsManager::CreateBackBuffer()
{
    // backBuffer -> Texture2D
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    
    HRESULT hr = swapChain->GetBuffer
    (
        0,
        __uuidof(ID3D11Texture2D),
        &backBuffer
    );
    CHECK(hr);

    // RenderTargetView 생성
    hr = device->CreateRenderTargetView
    (
        backBuffer.Get(),
        nullptr,
        &rtv
    );
    CHECK(hr);

    // Viewport 설정 (전역 윈도우 크기 사용)
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = gWinWidth;
    viewport.Height = gWinHeight;
    viewport.MinDepth = 0.0f;   // 가장 가까운 곳 (카메라 렌즈)
    viewport.MaxDepth = 1.0f;   // 가장 먼 곳 (카메라가 볼 수 있는 끝)
}

void GraphicsManager::Begin()
{
    // ViewPort 세팅, RenderTarget 바인드, back buffer 클리어
    deviceContext->RSSetViewports(1, &viewport);
    deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
    deviceContext->ClearRenderTargetView(rtv.Get(), clearColor);

    // BlendState 설정 (Alpha blending)
    deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);
}

void GraphicsManager::End()
{
    // 수직 동기화
    // - 0: 수직 동기화 OFF
    // - 1: 수직 동기화 ON
    HRESULT hr = swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
    //HRESULT hr = swapChain->Present(1,0);
    
    CHECK(hr);
}