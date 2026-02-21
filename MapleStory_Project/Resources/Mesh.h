#pragma once

//==============================================
// 렌더링용 메시 클래스
// 정점(Vertex) / 인덱스(Index) 버퍼 생성 및 관리
// GPU 바인딩을 통해 드로우 가능한 상태 제공
//==============================================
class Mesh
{
public:
	//======================================
	// 메시 버퍼 생성
	// 정점 배열과 인덱스 배열로 GPU 버퍼 생성
	// 템플릿을 사용하여 다양한 정점 구조 지원
	//======================================
	template<typename T>
	void Create(const std::vector<T>& vertices, const std::vector<UINT>& indices);

	// 메시 버퍼 바인딩
	void Bind();

	// 인덱스 버퍼 반환
	IndexBuffer* GetIndexBuffer() { return indexBuffer.get(); }

	// 인덱스 개수 반환
	UINT GetIndexCount() { return indexBuffer ? indexBuffer->GetCount() : 0; }

private:
	// GPU 정점 버퍼
	std::unique_ptr<VertexBuffer> vertexBuffer;
	// GPU 인덱스 버퍼
	std::unique_ptr<IndexBuffer> indexBuffer;
};

//===============================================================
// 템플릿 함수 구현
// inline 사용 이유
// 템플릿 함수는 정의가 헤더에 있어야 모든 타입에 대해 인스턴스화 가능
// 헤더 포함 시 중복 정의(ODR) 방지를 위해 inline 사용
//===============================================================
template<typename T>
inline void Mesh::Create(const std::vector<T>& vertices, const std::vector<UINT>& indices)
{
	// 정점 버퍼 생성 및 데이터 업로드
	vertexBuffer = std::make_unique<VertexBuffer>();
	vertexBuffer->Create(vertices);

	// 인덱스 버퍼 생성 및 데이터 업로드
	indexBuffer = std::make_unique<IndexBuffer>();
	indexBuffer->Create(indices);
}
