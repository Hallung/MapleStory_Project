#pragma once

// Vertex 데이터를 GPU 메모리에 생성하고 Input Assembler 단계에 바인딩하는 클래스
class VertexBuffer
{
public:
	// Vertex 데이터를 GPU에 업로드하여 VertexBuffer 생성
	// 템플릿으로 다양한 Vertex 구조체 타입 지원
	template <typename T>
	void Create(const std::vector<T>& vertices, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }

	// 생성된 VertexBuffer를 Input Assembler 단계에 바인딩
	void Bind();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer; // GPU에 생성된 VertexBuffer 리소스
	UINT stride = 0; // Vertex 하나의 크기 (byte) InputLayout 해석에 사용
	UINT offset = 0; // Vertex 시작 위치 오프셋
	UINT count = 0; // 전체 Vertex 개수
};

// template 함수는 특성상 헤더에 정의가 필요하며, inline은 중복 정의(ODR) 방지 및 인라이닝 힌트를 위해 관례적으로 함께 사용
template<typename T>
inline void VertexBuffer::Create(const std::vector<T>& vertices, D3D11_USAGE usage)
{
	stride = sizeof(T); // Vertex 구조체 하나의 크기 저장
	count = UINT(vertices.size()); // 전체 Vertex 개수 저장

	D3D11_BUFFER_DESC desc{}; // VertexBuffer 생성 설정 구조체

	desc.ByteWidth = stride * count; // 전체 버퍼 크기 = Vertex 크기 × 개수
	desc.Usage = usage; // Buffer 사용 방식 설정
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // VertexBuffer 용도로 사용됨을 명시

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
	default:
		break;
	}

	// GPU에 전달할 초기 Vertex 데이터
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = vertices.data();

	// VertexBuffer 생성 요청
	HRESULT hr = Graphics::GetInstance().GetDevice()->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);
}
