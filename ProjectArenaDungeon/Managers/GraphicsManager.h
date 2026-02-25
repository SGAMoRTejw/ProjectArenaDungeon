#pragma once

// GraphicsManager
// - DirectX11 Device, DeviceContext, SwapChain 및 렌더 타겟(RTV), 뷰포트를 관리
// - 렌더링 시작(Begin) / 종료(End) 제공
// - Sampler,Blend state 설정
// - Window 생성(gHandle 값) 이후 초기화
class GraphicsManager
{
	DECLARE_SINGLETON(GraphicsManager)

public:
	// Initialize
	// - D3D11Device, DeviceContext, SwapChain 생성
	// - Sampler, Belnd 상태 생성 및 BackBuffer 구성
	void Initialize();

	// CreateBackBuffer
	// - SwapChain의 back buffer에 RTV를 생성, viewport를 설정
	// - 윈도우 크기 변경 시 재생성이 필요할 수 있다.
	void CreateBackBuffer();

	// Begin
	// - 렌더링 프레임 시작: Viewport/RTV 바인딩 및 Clear 수행
	void Begin();
	
	// End
	// - 렌더링 프레임 종료: Present 호출
	void End();

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return deviceContext.Get(); }

	// Sampler
	// bPoint = true: Point sampling
	// bPoint = false: Linear sampling
	ID3D11SamplerState* GetSampler(bool bPoint) const { return bPoint ? samplerPoint.Get() : samplerLinear.Get(); }

	// SetSampler
	// - PixelShader 단계에 sampler 바인딩
	void SetSampler(bool bPoint)
	{
		deviceContext->PSSetSamplers(0, 1, bPoint ? samplerPoint.GetAddressOf() : samplerLinear.GetAddressOf());
	}

private:
	// D3D objects
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
	D3D11_VIEWPORT viewport{};
	
	// Clear color (배경색)
	DirectX::SimpleMath::Color clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };

	// states
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerPoint;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerLinear;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
};