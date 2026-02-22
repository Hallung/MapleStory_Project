#pragma once

//=======================================================================
// 머티리얼
// 렌더링에 필요한 셰이더 상태(InputLayout/VertexShader/PixelShader)를 보관
// Mesh와 함께 사용되어 렌더 파이프라인 상태를 구성
// 동일 셰이더 리소스를 여러 객체가 공유할 수 있도록 shared_ptr 사용
//=======================================================================
class Material
{
public:
	//===========================================
	// 셰이더 경로와 정점 레이아웃 정보를 기반으로
	// ShaderManager에서 셰이더 세트를 받아 초기화
	//===========================================
	Material(std::wstring shaderPath, std::span<const D3D11_INPUT_ELEMENT_DESC> layoutDesc);

	// 머티리얼 셰이더 상태를 렌더링 파이프라인에 바인딩
	void Bind();

private:
	std::shared_ptr<class InputLayout> inputLayout;	// 입력 정점 레이아웃
	std::shared_ptr<class VertexShader> vertexShader; // 정점 셰이더
	std::shared_ptr<class PixelShader> pixelShader;	// 픽셀 셰이더
};