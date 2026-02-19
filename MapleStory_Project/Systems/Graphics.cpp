#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
    Initialize();
}

// 필요한 것들을 만들고 초기화 하는 역할
void Graphics::Initialize()
{
    DXGI_SWAP_CHAIN_DESC desc{};

    // DXGI_MODE_DESC BufferDesc
    desc.BufferDesc.Width = 0;
    desc.BufferDesc.Height = 0;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // DXGI_SAMPLE_DESC SampleDesc
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    // DXGI_USAGE BufferUsage
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // UINT BufferCount
    desc.BufferCount = 2;

    // HWND OutputWindow
    desc.OutputWindow = gHandle;

    // BOOL Windowed
    desc.Windowed = true;

    // DXGI_SWAP_EFFECT SwapEffect
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    // UINT Flags
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    // BGRA를 켤 수 있도록 도와주는 DX 기능
    UINT flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    // DX에서 에러를 상세히 말해주는 옵션, 성능 저하가 조금 있다
    flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    std::vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL currentFeature;

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,                    // 기본 어뎁터사용
        D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 가속 사용(GPU 연산)
        nullptr,                    // 드라이브 타입 소프트웨어를 사용할 때 해당 부분을 넣기
        flag,                       // 플레그를 추가하거나, 디버그 단계에서 플레그를 설정할 경우 등등
        featureLevels.data(),
        UINT(featureLevels.size()),
        D3D11_SDK_VERSION,
        &desc,
        &swapChain,
        &device,
        nullptr,                    // 선택된 FEATURE_LEVEL을 볼 때 사용(D3D_FEATURE_LEVEL currentFeature; , &currentFeature)
        &deviceContext
    );
    CHECK(hr);                      // 성공 했는지 안했는지 확인 작업

    // 스케일에 변화에 따라 그림 뒤집기가 가능하도록 설정
    CD3D11_RASTERIZER_DESC rsDesc(D3D11_DEFAULT);
    rsDesc.CullMode = D3D11_CULL_NONE; // 앞, 뒷면을 모두 그리도록 설정
    hr = device->CreateRasterizerState(&rsDesc, &raterizerState);
    CHECK(hr);
    deviceContext->RSSetState(raterizerState.Get());

    // 알파값 적용
    CD3D11_BLEND_DESC blendDesc(D3D11_DEFAULT);
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    hr = device->CreateBlendState(&blendDesc, &blendState);
    CHECK(hr);

    // 샘플러 설정
    CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = device->CreateSamplerState(&samplerDesc, &samplerLinear);
    CHECK(hr);

    // 포인터로 변경하여 samplerPoint로 저장
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    hr = device->CreateSamplerState(&samplerDesc, &samplerPoint);
    CHECK(hr);

    deviceContext->PSSetSamplers(0, 1, samplerLinear.GetAddressOf());

    CreateBackBuffer();
}

// 실재 필요한 도화지를 만드는 역할
void Graphics::CreateBackBuffer()
{
    ComPtr<ID3D11Texture2D> backBuffer;

    HRESULT hr = swapChain->GetBuffer
    (
        0,
        __uuidof(ID3D11Texture2D),
        &backBuffer
    );
    CHECK(hr);

    hr = device->CreateRenderTargetView
    (
        backBuffer.Get(),
        nullptr,
        &renderTargetView
    );
    CHECK(hr);

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = gWinWidth;
    viewport.Height = gWinHeight;
    viewport.MinDepth = 0.0f;           // 가장 가까운곳(카메라 기준 카메라 렌즈 바로 앞)
    viewport.MaxDepth = 1.0f;           // 가장 먼곳(카메라 기준 카메라에서 가장 먼 지점)
}

void Graphics::Begin()
{
    deviceContext->RSSetViewports(1, &viewport);                                    // 뷰포트 설정
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr); // 렌더 타겟 지정
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);       // 화면 지우기
}

void Graphics::End()
{
    // BackBuffer에서 FrontBuffer로 넘겨주는 단계
    HRESULT hr = swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
    CHECK(hr);
}