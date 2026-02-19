#include "stdafx.h"
#include "InputLayout.h"

void InputLayout::Create(std::span<const D3D11_INPUT_ELEMENT_DESC> descs, const ComPtr<ID3DBlob>& blob)
{
	// descs 또는 shader blob이 없으면 생성 불가능
	if (descs.empty() || !blob.Get())
	{
		assert(false);
	}

	// descs와 blob을 통해 DirectX에 InputLayout 생성 요청
	// Shader와 Vertex 구조 일치 여부도 함께 검증
	HRESULT hr = Graphics::GetInstance().GetDevice()->CreateInputLayout(
		descs.data(),
		UINT(descs.size()),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout
	);
	CHECK(hr);
}

void InputLayout::Bind()
{
	// 생성된 InputLayout을 Input Assembler 단계에 바인딩 (이 구조 기준으로 VertexBuffer 데이터 해석)
	Graphics::GetInstance().GetDeviceContext()->IASetInputLayout(inputLayout.Get());
}
