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

	DirectX::SimpleMath::Color GetColor() const
	{
		if (colorBuffer) return colorBuffer->GetColor();
		return DirectX::SimpleMath::Color(1, 1, 1, 1);
	}

	// 머티리얼 색상(ConstantBuffer) 설정
	void SetColor(DirectX::SimpleMath::Color color)
	{
		if (colorBuffer) colorBuffer->SetColor(color);
	}

	// Texture Atlas의 타일 그리드 정보 설정 (atlasIndex -> UV 좌표 계산에 사용)
	void SetAtlasGrid(UINT cols, UINT rows)
	{
		atlasCols = cols;
		atlasRows = rows;
	}

	int GetAtlasIndex() const { return atlasIndex; }

	// Atlas 타일 인덱스 설정
	void SetAtlasIndex(int index) 
	{ 
		// 현재 사용할 Atlas 타일 인덱스 저장
		atlasIndex = index;

		// FrameBuffer가 없거나 Atlas 정보가 설정되지 않은 경우 처리하지 않음
		if (!frameBuffer) return;
		if (atlasCols == 0 || atlasRows == 0) return;

		// Atlas 인덱스를 타일 그리드 좌표(x, y)로 변환
		UINT x = atlasIndex % atlasCols;
		UINT y = atlasIndex / atlasCols;

		// Atlas 한 칸의 UV 크기 계산
		float uvWidth = 1.0f / atlasCols;
		float uvHeight = 1.0f / atlasRows;

		// 해당 타일의 UV 시작 위치 계산
		DirectX::SimpleMath::Vector2 offset = { x * uvWidth, y * uvHeight };
		// 해당 타일의 UV 영역 크기
		DirectX::SimpleMath::Vector2 size = { uvWidth, uvHeight };

		// 계산된 UV 정보를 FrameBuffer에 전달하여 셰이더에서 사용
		frameBuffer->SetFrameData(offset, size);
	}

	//======================================
	// 프레임(애니메이션 UV) 데이터 버퍼 반환
	// 셰이더에 전달되는 FrameBuffer 접근용
	//======================================
	const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return frameBuffer; }

	//======================================================
	// Point 샘플링 사용 여부 설정
	// true : Point Sampling (픽셀 아트, 선명한 텍스처 유지)
	// false : Linear Sampling (부드러운 보간)
	//======================================================
	void SetPointSampler(bool value) { bPointSampling = value; }

private:
	std::shared_ptr<class InputLayout> inputLayout;	// 입력 정점 레이아웃
	std::shared_ptr<class VertexShader> vertexShader; // 정점 셰이더
	std::shared_ptr<class PixelShader> pixelShader;	// 픽셀 셰이더

	std::shared_ptr<Texture> texture;	// 머티리얼에 바인딩될 텍스처 리소스
	std::shared_ptr<FrameBuffer> frameBuffer; // UV 프레임/애니메이션 정보를 담는 상수 버퍼

	std::unique_ptr<ColorBuffer> colorBuffer;	// 머티리얼 색상 정보를 GUP에 전달하는 상수 버퍼

	bool bPointSampling = false;	// 현재 샘플러 필터링 모드 상태

	UINT atlasCols = 1; // 타일셋의 가로 타일 개수
	UINT atlasRows = 1; // 타일셋의 세로 타일 개수
	int atlasIndex = 0; // 현재 사용할 타일 인덱스
};