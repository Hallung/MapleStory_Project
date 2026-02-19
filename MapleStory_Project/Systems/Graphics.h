#pragma once

using Microsoft::WRL::ComPtr;

class Graphics
{
private:
	Graphics();
	~Graphics() = default;

public:
	Graphics(const Graphics& otherGraphics) = delete;
	Graphics& operator=(const Graphics& otherGraphics) = delete;
	Graphics(Graphics&& otherGraphics) = delete;
	Graphics& operator=(Graphics&& otherGraphics) = delete;

	static Graphics& GetInstance()
	{
		static Graphics instance;
		return instance;
	}

public:
	void Initialize();
	void CreateBackBuffer();

	void Begin();
	void End();

	ComPtr<ID3D11Device> GetDevice() const { return device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return deviceContext; }

private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RasterizerState> raterizerState;
	D3D11_VIEWPORT viewport;
	// 초기화할 색상
	DirectX::SimpleMath::Color clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };
};