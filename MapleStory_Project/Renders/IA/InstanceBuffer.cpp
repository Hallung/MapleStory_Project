#include "stdafx.h"
#include "InstanceBuffer.h"

// 인스턴스 버퍼 생성
void InstanceBuffer::Create(UINT maxCount, UINT stride)
{
	this->maxCount = maxCount;
	this->stride = stride;

	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 매 프레임 데이터를 갱신하기 위한 Dynamic 버퍼
	desc.ByteWidth = stride * maxCount; // 버퍼 전체 크기
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Vertex Buffer로 사용 (Instance Data)
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU에서 Write 가능

	HRESULT hr = Graphics::GetInstance().GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	CHECK(hr);
}

// 인스턴스 데이터 업데이트
void InstanceBuffer::Update(const void* data, UINT count)
{
	this->count = count;
	if (count == 0) return;

	D3D11_MAPPED_SUBRESOURCE subResource;

	// 기존 데이터를 버리고 새 데이터를 쓰기 위해 WRITE_DISCARD 사용
	HRESULT hr = Graphics::GetInstance().GetDeviceContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	CHECK(hr);

	// CPU 메모리의 인스턴스 데이터를 GPU 버퍼로 복사
	memcpy(subResource.pData, data, stride * count);

	Graphics::GetInstance().GetDeviceContext()->Unmap(buffer.Get(), 0);
}

// Input Assembler 단계에 인스턴스 버퍼 바인딩
void InstanceBuffer::Bind(UINT slot)
{
	if (count == 0) return;

	Graphics::GetInstance().GetDeviceContext()->IASetVertexBuffers(slot, 1, buffer.GetAddressOf(), &stride, &offset);
}