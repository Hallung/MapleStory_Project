#include "stdafx.h"
#include "VertexType.h"

std::vector<D3D11_INPUT_ELEMENT_DESC> Color::descs
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

//======================================
// VertexTexture 입력 레이아웃 정의
// 텍스처 샘플링 셰이더 입력용 정점 포맷
//======================================
std::vector<D3D11_INPUT_ELEMENT_DESC> VertexTexture::descs
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

//======================================
// VertexInstancing 입력 레이아웃 정의
// 인스턴싱 셰이더 입력용 인스턴스 데이터 포맷
//======================================
std::vector<D3D11_INPUT_ELEMENT_DESC> VertexInstancing::descs
{
	// Matrix format이 없으므로 float4 4개로 분리하여 전달
	{ "INST_WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "INST_WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "INST_WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "INST_WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	// Atlas에서 사용할 스프라이트 인덱스
	{ "INST_INDEX", 0, DXGI_FORMAT_R32_SINT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};