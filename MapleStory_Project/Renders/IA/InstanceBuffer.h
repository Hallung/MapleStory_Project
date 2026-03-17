#pragma once

// GPU Instancing을 위한 인스턴스 데이터 버퍼
// 각 인스턴스의 Transform, Index 등의 데이터를 GPU로 전달하기 위해 사용
class InstanceBuffer
{
public:
	// 인스턴스 버퍼 생성
	// maxCount : 저장 가능한 최대 인스턴스 개수
	// stride : 인스턴스 하나의 데이터 크기
	void Create(UINT maxCount, UINT stride);

	// 인스턴스 데이터 업데이트
	// data : CPU에서 생성한 인스턴스 데이터 배열
	// count : 실제 사용할 인스턴스 개수
	void Update(const void* data, UINT count);

	// Input Assembler 단계에 인스턴스 버퍼 바인딩
	// slot : VertexBuffer 슬롯
	void Bind(UINT slot = 1);

	UINT GetMaxCount() const { return maxCount; }
	UINT GetCount() const { return count; }

private:
	ComPtr<ID3D11Buffer> buffer; // GPU 인스턴스 버퍼

	UINT maxCount = 0; // 최대 인스턴스 개수
	UINT count = 0;	   // 현재 사용 중인 인스턴스 개수
	UINT stride = 0;   // 인스턴스 데이터 크기
	UINT offset = 0;   // VertexBuffer 바인딩 offset
};