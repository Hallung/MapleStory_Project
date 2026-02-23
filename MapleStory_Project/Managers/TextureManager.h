#pragma once

class Texture;

//===============================
// 텍스처 리소스 관리자
// Texture 로드 및 공유 관리
// 동일 경로 텍스처 중복 생성 방지
// 전역에서 싱글턴으로 접근
//===============================
class TextureManager
{
private:
	TextureManager();
	~TextureManager() = default;

public:
	// 싱글턴 복사/이동 금지
	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;

	// 싱글턴 인스턴스 반환
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

public:
	//=====================================
	// 텍스처 로드 또는 캐시 반환
	// 이미 로드된 경로면 기존 Texture 공유
	// 없으면 새 Texture 생성 후 캐시 등록
	//=====================================
	std::shared_ptr<Texture> LoadTexture(const std::wstring& path);

private:
	// 경로->Texture 캐시(동일 텍스처 리소스 공유)
	std::unordered_map<std::wstring, std::shared_ptr<Texture>> cache;
};