#pragma once

// Index 데이터를 GPU 메모리에 생성하고 Input Assembler 단계에 바인딩하는 클래스
// VertexBuffer와 함께 사용되어 정점의 그리는 순서를 결정한다
class IndexBuffer
{
public:
	// Index 데이터를 GPU에 업로드하여 IndexBuffer 생성
	void Create(const std::vector<UINT>& indices, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }

	// 생성된 IndexBuffer를 Input Assembler 단계에 바인딩
	void Bind();

private:
	ComPtr<ID3D11Buffer> buffer; // GPU에 생성된 IndexBuffer 리소스
	UINT stride = 0; // Index 하나의 크기 (byte)
	UINT offset = 0; // Index 시작 위치 오프셋
	UINT count = 0; // 전체 Index 개수
};


