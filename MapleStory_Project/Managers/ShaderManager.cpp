#include "stdafx.h"
#include "Renders/IA/InputLayout.h"
#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"
#include "ShaderManager.h"

// =================================================
// Singleton 패턴을 위한 생성자
// GetInstance()에서 내부 static instance 생성 시 호출
// =================================================
ShaderManager::ShaderManager() {}

//================================================
// 셰이더 요청
// 캐시에 존재하면 재사용
// 없으면 셰이더 및 입력 레이아웃 생성 후 캐시에 등록
//================================================
ShaderSet ShaderManager::GetShader(const std::wstring& path, std::span<const D3D11_INPUT_ELEMENT_DESC> descs)
{
	// 캐시에서 셰이더 조회
	auto it = shaderCache.find(path);
	if (it != shaderCache.end())
		return it->second;	// 이미 생성된 셰이더 재사용

	// 새로운 셰이더 세트 생성
	ShaderSet newSet;

	// 버텍스 셰이더 생성 (엔트리 : VS)
	newSet.vertexShader = std::make_shared<VertexShader>();
	newSet.vertexShader->Create(path, "VS");

	// 입력 레이아웃 생성 (descs, Blob)
	newSet.inputLayout = std::make_shared<InputLayout>();
	newSet.inputLayout->Create(descs, newSet.vertexShader->GetBlob());

	// 픽셀 셰이더 생성 (엔트리 : PS)
	newSet.pixelShader = std::make_shared<PixelShader>();
	newSet.pixelShader->Create(path, "PS");

	// 캐시에 등록 (path 기준)
	shaderCache.emplace(path, newSet);

	return newSet;
}
