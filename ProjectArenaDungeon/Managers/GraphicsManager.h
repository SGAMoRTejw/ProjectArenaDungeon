#pragma once

class GraphicsManager
{
	DECLARE_SINGLETON(GraphicsManager)

public:
	void Initialize();
	void CreateBackBuffer();

	void Begin();		// 그릴 곳 세팅, 초기화, 뷰포트 설정. rendering pipeline 가동
	void End();

	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return deviceContext.Get(); }

	// bPoint 값에 따라 샘플링 변경(true: Point / false: Linear)
	ID3D11SamplerState* GetSampler(bool bPoint) const { return bPoint ? samplerPoint.Get() : samplerLinear.Get(); }

	void SetSampler(bool bPoint)
	{
		deviceContext->PSSetSamplers(0, 1, bPoint ? samplerPoint.GetAddressOf() : samplerLinear.GetAddressOf());
	}

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> rtv;
	D3D11_VIEWPORT viewport{};
	
	// clear에 사용할 배경색
	Color clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };

	ComPtr<ID3D11SamplerState> samplerPoint;
	ComPtr<ID3D11SamplerState> samplerLinear;
	ComPtr<ID3D11BlendState> blendState;
};