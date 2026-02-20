#include "stdafx.h"
#include "IndexBuffer.h"

void IndexBuffer::Create(const std::vector<UINT>& indices, D3D11_USAGE usage)
{
	stride = sizeof(UINT); // Index 구조체 하나의 크기 저장
	count = UINT(indices.size()); // 전체 Index 개수 저장

	D3D11_BUFFER_DESC desc{}; // IndexBuffer 생성 설정 구조체

	desc.ByteWidth = stride * count; // 전체 버퍼 크기 = Index 크기 × 개수
	desc.Usage = usage; // Buffer 사용 방식 설정
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // IndexBuffer 용도로 사용됨을 명시

	// Usage 타입에 따라 CPU 접근 권한 설정
	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		break;
	case D3D11_USAGE_IMMUTABLE:
		// 생성 후 수정 불가
		desc.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_DYNAMIC:
		// CPU에서 수정 가능
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		// CPU 읽기/쓰기 모두 가능
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	}

	// GPU에 전달할 초기 Index 데이터
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = indices.data();

	// IndexBuffer 생성 요청
	HRESULT hr = Graphics::GetInstance().GetDevice()->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);
}

void IndexBuffer::Bind()
{
	// 생성된 IndexBuffer를 Input Assembler 단계에 바인딩
	Graphics::GetInstance().GetDeviceContext()->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}