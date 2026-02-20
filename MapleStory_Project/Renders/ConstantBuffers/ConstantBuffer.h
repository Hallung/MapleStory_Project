#pragma once
#include "stdafx.h"

// Shader에서 사용하는 상수 데이터(Constant Buffer)를 GPU 메모리에 생성하고 VertexShader / PixelShader 단계에 바인딩하는 클래스
// Shader에 값을 전달하는 GPU 메모리
template <typename T>
class ConstantBuffer
{
public:
	virtual ~ConstantBuffer() = default;

	// CPU에서 수정된 데이터를 GPU ConstantBuffer에 업로드
	// bDirty가 true일 때만 업데이트 수행하여 불필요한 Map 호출 방지
	void Update()
	{
		// 변경된 데이터가 없으면 업데이트하지 않음
		if (!bDirty) return;

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;

		// GPU 메모리를 CPU에서 접근 가능하도록 매핑
		HRESULT hr = Graphics::GetInstance().GetDeviceContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		CHECK(hr);
		{
			// CPU data를 GPU ConstantBuffer 메모리에 복사
			memcpy(mappedSubResource.pData, &data, sizeof(T));
		}
		// 매핑 해제하여 GPU가 다시 사용할 수 있도록 함
		Graphics::GetInstance().GetDeviceContext()->Unmap(buffer.Get(), 0);

		// 업데이트 완료 상태로 변경
		bDirty = false;
	}

	// ConstantBuffer를 VertexShader 단계에 바인딩
	void BindVS(UINT slot)
	{
		Graphics::GetInstance().GetDeviceContext()->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

	// ConstantBuffer를 PixelShader 단계에 바인딩
	void BindPS(UINT slot)
	{
		Graphics::GetInstance().GetDeviceContext()->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

protected:
	// ConstantBuffer 생성
	// GPU에 ConstantBuffer 리소스를 생성하고 CPU에서 수정 가능하도록 설정
	ConstantBuffer()
	{
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = (sizeof(T) + 15) & ~15; // ConstantBuffer는 반드시 16byte 정렬 필요
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU에서 Write 가능하도록 설정 

		HRESULT hr = Graphics::GetInstance().GetDevice()->CreateBuffer(&desc, nullptr, &buffer);
		CHECK(hr);
	}

protected:
	T data; // Shader에 전달할 실제 데이터

	bool bDirty = true; // 데이터 변경 여부 (true일 때만 GPU 업데이트 수행)

private:
	ComPtr<ID3D11Buffer> buffer; // GPU에 생성된 ConstantBuffer 리소스
};