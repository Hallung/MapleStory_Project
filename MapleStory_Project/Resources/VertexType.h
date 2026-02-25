#pragma once

//===============================
// 정점 타입 : 위치 + 색상
// 컬러 렌더링용 기본 2D 정점 구조
//===============================
struct Vertex
{
	Vertex() = default;
	Vertex(DirectX::SimpleMath::Vector2 position)
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