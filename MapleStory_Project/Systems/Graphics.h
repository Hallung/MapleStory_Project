#pragma once

using Microsoft::WRL::ComPtr;

// DirectX11 Graphics 시스템 관리 싱글턴 클래스
class Graphics
{
private:
	Graphics();
	~Graphics() = default;

public:
	// 싱글턴 복사/이동 금지
	Graphics(const Graphics& otherGraphics) = delete;
	Graphics& operator=(const Graphics& otherGraphics) = delete;
	Graphics(Graphics&& otherGraphics) = delete;
	Graphics& operator=(Graphics&& otherGraphics) = delete;

	// 싱글턴 인스턴스 반환
	static Graphics& GetInstance()
	{
		static Graphics instance;
		return instance;
	}

public:
	void Initialize(); // DirectX Device 및 SwapChain 초기화
	void CreateBackBuffer(); // BackBuffer 생성 및 RenderTargetView 생성

	void Begin();
	void End();

	ComPtr<ID3D11Device> GetDevice() const { return device; } // GPU 리소스 생성 시 사용
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return deviceContext; } // GPU 렌더링 명령 실행 시 사용

	//==================================================================
	// 현재 설정된 샘플러 상태 반환
	// bPoint == true : Point 샘플링 반환 (픽셀 아트 등 선명한 텍스처용)
	// bPoint == false : Linear 샘플링 반환 (일반 텍스처 보간용)
	//==================================================================
	ID3D11SamplerState* GetSample(bool bPoint) const { return bPoint ? samplerPoint.Get() : samplerLinear.Get(); }

	//================================================================
	// Pixel Shader에 샘플러 상태 바인딩
	// bPoint 값에 따라 Point / Linear 샘플러를 선택하여 slot 0 에 설정
	// 이후 Pixel Shader에서 텍스처 샘플링 시 해당 필터 방식이 적용
	//================================================================
	void SetSampler(bool bPoint)
	{
		deviceContext->PSSetSamplers(0, 1, bPoint ? samplerPoint.GetAddressOf() : samplerLinear.GetAddressOf());
	}

private:
	ComPtr<ID3D11Device> device; // GPU 리소스 생성 담당
	ComPtr<ID3D11DeviceContext> deviceContext; // GPU 렌더링 명령 실행 담당
	ComPtr<ID3D11RenderTargetView> renderTargetView; // BackBuffer를 렌더링 대상으로 사용
	ComPtr<IDXGISwapChain> swapChain; // BackBuffer와 FrontBuffer 교체 담당
	ComPtr<ID3D11RasterizerState> raterizerState; // 컬링, 채우기 모드 등 설정
	ComPtr<ID3D11BlendState> blendState; // Alpha Blend 등 색상 혼합 설정
	ComPtr<ID3D11SamplerState> samplerPoint; // 픽셀 단위 샘플링
	ComPtr<ID3D11SamplerState> samplerLinear; // 부드러운 텍스처 샘플링
	D3D11_VIEWPORT viewport; // 렌더링 영역 설정

	// 초기화할 색상
	DirectX::SimpleMath::Color clearColor = { 0.4f, 0.4f, 0.4f, 1.0f };
};