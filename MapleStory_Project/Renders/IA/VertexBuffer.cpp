#include "stdafx.h"
#include "VertexBuffer.h"

void VertexBuffer::Bind()
{
	// 생성된 VertexBuffer를 Input Assembler 단계에 바인딩
	// GPU가 Vertex 데이터를 읽을 수 있도록 설정
	Graphics::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
}
