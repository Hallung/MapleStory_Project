#pragma once

//===============================
// 정점 타입 : 위치 + 색상
// 컬러 렌더링용 기본 2D 정점 구조
//===============================
struct Color
{
	Color() = default;
	Color(DirectX::SimpleMath::Vector2 position)
		:position(position)
	{
	}

	DirectX::SimpleMath::Vector2 position;

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

//==============================
// 정점 타입 : 위치 + 텍스처 좌표
// 텍스처 렌더링용 2D 정점 구조
//==============================
struct VertexTexture
{
	VertexTexture() = default;
	// 위치 + UV 초기화 생성자
	VertexTexture(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 uv)
		: position(position), uv(uv)
	{
	}

	DirectX::SimpleMath::Vector2 position;	// 정점 위치
	DirectX::SimpleMath::Vector2 uv;		// 텍스처 좌표

	//==============================================
	// 입력 레이아웃 정의 (InputLayout 생성용)
	// 중복 메모리 생성 방지를 위해 static으로 선언
	//==============================================
	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

//==============================
// 인스턴싱 데이터 타입
// GPU Instancing 렌더링을 위한 인스턴스 데이터 구조
//==============================
struct VertexInstancing
{
	DirectX::SimpleMath::Matrix world; // 각 인스턴스의 월드 변환 행렬
	int textureIndex = 0;			   // 사용할 스프라이트 Atlas 인덱스

	//==============================================
	// 인스턴스 입력 레이아웃 정의 (InputLayout 생성용)
	// Matrix 타입은 직접 전달할 수 없으므로 float4 4개로 분리하여 전달
	//==============================================
	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};