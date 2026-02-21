#include "stdafx.h"
#include "Renders/IA/InputLayout.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "Material.h"

//=========================================================================
// 머티리얼 생성
// ShaderManager에서 ShaderSet(InputLayout/VertexShader/PixelShader)을 요청
// 동일 셰이더 리소스를 공유하여 중복 생성 방지
//=========================================================================
Material::Material(std::wstring shaderPath, std::span<const D3D11_INPUT_ELEMENT_DESC> layoutDesc)
{
	ShaderSet shaders = ShaderManager::GetInstance().GetShader(shaderPath, layoutDesc);
	inputLayout = shaders.inputLayout;
	vertexShader = shaders.vertexShader;
	pixelShader = shaders.pixelShader;
}

//==================================
// 머티리얼 바인딩
// 렌더링 파이프라인 셰이더 상태 설정
// IA 단계: InputLayout
// VS 단계: VertexShader
// PS 단계: PixelShader
//==================================
void Material::Bind()
{
	if (inputLayout) inputLayout->Bind();
	if (vertexShader) vertexShader->Bind();
	if (pixelShader) pixelShader->Bind();
}
