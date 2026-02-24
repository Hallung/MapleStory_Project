#pragma once
//==================================================================
// ColorBuffer를 Material이 직접 소유(unique_ptr)하므로 완전 타입 필요
// 전방선언으로는 불가하여 헤더에서 include
//==================================================================
#include "Renders/ConstantBuffers/GlobalBuffers.h"

class Texture;

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

	//=================================
	// 텍스처 설정
	// 머티리얼에 사용할 Texture 지정
	// shared_ptr로 이동하여 소유권 이전
	//=================================
	void SetTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = move(texture);
	}

	// 머티리얼 색상(ConstantBuffer) 설정
	void SetColor(DirectX::SimpleMath::Color color)
	{
		if (colorBuffer) colorBuffer->SetColor(color);
	}

	//======================================
	// 프레임(애니메이션 UV) 데이터 버퍼 반환
	// 셰이더에 전달되는 FrameBuffer 접근용
	//======================================
	const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return frameBuffer; }

private:
	std::shared_ptr<class InputLayout> inputLayout;	// 입력 정점 레이아웃
	std::shared_ptr<class VertexShader> vertexShader; // 정점 셰이더
	std::shared_ptr<class PixelShader> pixelShader;	// 픽셀 셰이더

	std::shared_ptr<Texture> texture;	// 머티리얼에 바인딩될 텍스처 리소스
	std::shared_ptr<FrameBuffer> frameBuffer; // UV 프레임/애니메이션 정보를 담는 상수 버퍼

	std::unique_ptr<ColorBuffer> colorBuffer;	// 머티리얼 색상 정보를 GUP에 전달하는 상수 버퍼
};