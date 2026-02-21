#include "stdafx.h"
#include "Renders/IA/VertexBuffer.h"
#include "Renders/IA/IndexBuffer.h"
#include "Mesh.h"

//====================================================
// 메시 버퍼 바인딩
// VertexBuffer와 IndexBuffer를 렌더링 파이프라인에 설정
// IA 단계 입력 레이아웃 구성
//====================================================
void Mesh::Bind()
{
	// 정점 버퍼가 존재하면 바인딩
	if (vertexBuffer)
		vertexBuffer->Bind();

	// 인덱스 버퍼가 존재하면 바인딩
	if (indexBuffer)
		indexBuffer->Bind();
}