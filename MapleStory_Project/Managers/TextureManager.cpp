#include "stdafx.h"
#include "TextureManager.h"

// =================================================
// Singleton 패턴을 위한 생성자
// GetInstance()에서 내부 static instance 생성 시 호출
// =================================================
TextureManager::TextureManager() {}

//==================================
// 텍스처 로드
// 캐시에 존재하면 기존 Texture 반환
// 없으면 Texture 생성 후 캐시에 저장
//==================================
std::shared_ptr<Texture> TextureManager::LoadTexture(const std::wstring& path)
{
	// 이미 로드된 텍스처면 재사용
	if (cache.count(path) > 0)
		return cache[path];

	// 새 Texture 생성
	auto texture = std::make_shared<Texture>(path);

	// 캐시에 등록
	cache[path] = texture;

	return texture;
}
