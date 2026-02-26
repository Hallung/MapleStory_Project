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