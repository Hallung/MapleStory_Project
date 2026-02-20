#pragma once
#include <span>
// InputLayout 생성 시 Vertex 구조 배열을 복사없이 안전하게 전달하기 위해 데이터를 소유하지 않고 참조만 하는 View 객체 std::span 사용

// VertexBuffer의 데이터 구조를 정의하여 VertexShader가 올바르게 해석할 수 있도록 연결하는 역할
class InputLayout
{
public:
	// InputLayout 생성
	void Create(std::span<const D3D11_INPUT_ELEMENT_DESC> descs, const ComPtr<ID3DBlob>& blob);

	// Input Assembler 단계에 바인딩
	void Bind();

	ID3D11InputLayout* GetResource() const { return inputLayout.Get(); }

private:
	// Vertex 데이터 해석 방식을 GPU에 전달하는 리소스
	ComPtr<ID3D11InputLayout> inputLayout;
};