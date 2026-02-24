#include "stdafx.h"
#include "Renders/IA/InputLayout.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "Material.h"
#include "Texture.h"

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

	// 프레임 데이터용 상수버퍼 생성
	frameBuffer = std::make_shared<FrameBuffer>();
	// 기본 UV 영역 전체(0 ~ 1)로 초기화(애니메이션 미사용 시 전체 텍스처 출력)
	frameBuffer->SetFrameData({ 0, 0 }, { 1, 1 });
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

	// 텍스처 바인딩 (PS 슬롯 0)
	if (texture) texture->Bind(0);

	// 프레임(UV) 데이터 셰이더 전달
	if (frameBuffer)
	{
		// CPU -> GPU 상수 버퍼 업데이트
		frameBuffer->Update();
		// VS 슬롯 2에 바인딩
		frameBuffer->BindVS(2);
	}
}
